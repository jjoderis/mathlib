#ifndef MATHLIB_CORE_VECTOR_VECTOR_POINT_BASE_TEMPLATE
#define MATHLIB_CORE_VECTOR_VECTOR_POINT_BASE_TEMPLATE

#include "../../util/type_traits.h"
#include "../../util/util.h"
#include <cassert>
#include <iostream>
#include <math.h>
#include <type_traits>

namespace MathLib
{
// A base template class containing code to represent vectors and points
template <typename T, int numElements, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class VectorPointBase
{
protected:
    T *m_data = nullptr;
    friend class VectorPointBase<T, numElements - 1>;
    friend class VectorPointBase<T, numElements + 1>;

public:
    VectorPointBase() { m_data = new T[numElements]; }

    ~VectorPointBase()
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
        }

        m_data = nullptr;
    }

    // provide constructor that is only callable with correct number of parameters
    template <typename... Tail>
    VectorPointBase(
        typename std::enable_if<sizeof...(Tail) + 1 == numElements && are_arithmetic<Tail...>{}, T>::type head,
        Tail... tail)
        : m_data{new T[numElements]{head, T(tail)...}}
    {
    }

    VectorPointBase(const VectorPointBase &other) : VectorPointBase() { *this = other; }

    // copy construction and assignment with conversion
    template <typename U>
    VectorPointBase(const VectorPointBase<U, numElements> &other) : VectorPointBase()
    {
        *this = other;
    }

    // move construction
    VectorPointBase(VectorPointBase &&other) { *this = std::move(other); }

    // create vector with size: numElements + 1 by providing a vector with size: numElements and an additional number
    VectorPointBase(const VectorPointBase<T, numElements - 1> &other, T val) : VectorPointBase()
    {
        for (int i = 0; i < numElements; ++i)
        {
            m_data[i] = other.m_data[i];
        }

        m_data[numElements - 1] = val;
    }

    // create vector with size: numElements - 1 from a vector which will remove the last entry
    VectorPointBase(const VectorPointBase<T, numElements + 1> &other) : VectorPointBase()
    {
        for (int i = 0; i < numElements; ++i)
        {
            m_data[i] = other.m_data[i];
        }
    }

    VectorPointBase<T, numElements> &operator=(const VectorPointBase<T, numElements> &other)
    {
        if (this == &other)
        {
            return *this;
        }

        for (int i = 0; i < numElements; ++i)
        {
            m_data[i] = other.m_data[i];
        }

        return *this;
    }

    VectorPointBase &operator=(VectorPointBase &&other)
    {
        this->m_data = other.m_data;
        other.m_data = nullptr;

        return *this;
    }

    constexpr int size() const { return numElements; };

    // using () operator for subscript to have same API as matrix ([] can't take two arguments)
    T operator()(int index) const { return m_data[index]; }

    T &operator()(int index) { return m_data[index]; }

    // access to index which out of bounds check
    T at(int index) const
    {
        assert("Accessing out of bounds index" && index >= 0 && index < numElements);

        return m_data[index];
    }

    T &at(int index)
    {
        assert("Accessing out of bounds index" && index >= 0 && index < numElements);

        return m_data[index];
    }

    // update of value at index with out of bound check
    void set(int index, T val)
    {
        assert("Accessing out of bounds index" && index >= 0 && index < numElements);

        m_data[index] = val;
    }

    // return a pointer to the internal data array
    T *data() const { return m_data; }
};

std::false_type is_point_or_vector_impl(...);
template <typename T, int size>
std::true_type is_point_or_vector_impl(VectorPointBase<T, size> const volatile &);

template <typename T>
using is_point_or_vector = decltype(is_point_or_vector_impl(std::declval<T &>()));

template <typename U, typename T>
typename std::enable_if<is_point_or_vector<U>::value && std::is_arithmetic<T>::value, U>::type &operator+=(U &vp, T val)
{
    for (int i = 0; i < vp.size(); ++i)
    {
        vp(i) += val;
    }

    return vp;
}

template <typename U, typename T>
typename std::enable_if<is_point_or_vector<U>::value, U>::type &operator-=(U &vp, T val)
{
    for (int i = 0; i < vp.size(); ++i)
    {
        vp(i) -= val;
    }

    return vp;
}

template <typename U>
typename std::enable_if<is_point_or_vector<U>::value, U>::type &operator*=(U &vp1, const U &vp2)
{
    for (int i = 0; i < vp1.size(); ++i)
    {
        vp1(i) *= vp2(i);
    }

    return vp1;
}

template <typename U, typename T>
typename std::enable_if<is_point_or_vector<U>::value && std::is_arithmetic<T>::value, U>::type &operator*=(U &vp, T val)
{
    for (int i = 0; i < vp.size(); ++i)
    {
        vp(i) *= val;
    }

    return vp;
}

template <typename U, typename T>
typename std::enable_if<is_point_or_vector<U>::value, U>::type &operator/=(U &vp, T val)
{
    for (int i = 0; i < vp.size(); ++i)
    {
        vp(i) /= val;
    }

    return vp;
}

template <typename U, typename V>
typename std::enable_if<is_point_or_vector<U>::value && std::is_arithmetic<V>::value, U>::type operator*(V val,
                                                                                                         const U &vp)
{
    U product{vp};

    return product *= val;
}

template <typename U, typename V>
typename std::enable_if<is_point_or_vector<U>::value, U>::type operator*(const U &vp, V val)
{
    U product{vp};

    return product *= val;
}

template <typename U, typename V>
typename std::enable_if<is_point_or_vector<U>::value, U>::type operator/(const U &vp, V val)
{
    U product{vp};

    return product /= val;
}

template <typename U, typename V>
typename std::enable_if<is_point_or_vector<U>::value, U>::type operator/(V val, const U &vp)
{
    U product{};

    for (int i = 0; i < vp.size(); ++i)
    {
        product(i) = val / vp(i);
    }

    return product;
}

template <typename U>
typename std::enable_if<is_point_or_vector<U>::value, U>::type operator-(const U &vp)
{
    U negation{vp};

    return negate(negation);
}

template <typename U>
typename std::enable_if<is_point_or_vector<U>::value, U>::type &negate(U &vp)
{
    for (int i = 0; i < vp.size(); ++i)
    {
        vp(i) *= -1;
    }

    return vp;
}

template <typename U>
typename std::enable_if<is_point_or_vector<U>::value, U>::type &clamp(U &vp, const U &min, const U &max)
{
    for (int i = 0; i < vp(i); ++i)
    {
        vp(i) = (vp(i) < min) ? min : vp(i);
        vp(i) = (vp(i) > max) ? max : vp(i);
    }

    return vp;
}

template <typename T>
float param_sum_impl(float param, const T &vp)
{
    return param;
}

template <typename T, typename... Args>
float param_sum_impl(float param, const T &vp, const Args &...args)
{
    return param + param_sum_impl(args...);
}

template <typename T>
void combine_point_or_vector_impl(T &combination, float param, const T &vp)
{
    for (int i{0}; i < combination.size(); ++i)
    {
        combination(i) += param * vp(i);
    }
}

template <typename T, typename... Args>
void combine_point_or_vector_impl(T &combination, float param, const T &vp, const Args &...args)
{
    for (int i{0}; i < combination.size(); ++i)
    {
        combination(i) += param * vp(i);
    }

    combine_point_or_vector_impl(combination, args...);
}

template <typename T, typename... Args>
T affineCombination(float param, const T &vp, const Args &...args)
{
    static_assert("Called affineCombination with uneven number of arguments. Should be called with"
                  "(Parameter, Point/Vector, Parameter, ...)" &&
                  sizeof...(Args) % 2 == 0);
    float paramSum{param_sum_impl(param, vp, args...)};
    assert("Parameters are not summing up to 1." && Util::isClose(paramSum, 1.0f));

    T combination{vp};
    combination *= param;

    combine_point_or_vector_impl(combination, args...);

    return combination;
}

template <typename T, int size>
bool operator==(const VectorPointBase<T, size> &v1, const VectorPointBase<T, size> &v2)
{
    for (int i = 0; i < size; ++i)
    {
        if (v1(i) != v2(i))
        {
            return false;
        }
    }

    return true;
}

template <typename T, int size, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
bool allClose(const VectorPointBase<T, size> &v1,
              const VectorPointBase<T, size> &v2,
              T maxDiff = std::numeric_limits<T>::epsilon(),
              T maxRelDiff = std::numeric_limits<T>::epsilon())
{
    for (int i = 0; i < size; ++i)
    {
        if (!Util::isClose(v1(i), v2(i), maxDiff, maxRelDiff))
        {
            return false;
        }
    }

    return true;
}

template <typename T, int size>
bool operator!=(const VectorPointBase<T, size> &v1, const VectorPointBase<T, size> &v2)
{
    return !(v1 == v2);
}

template <typename T, int size>
std::ostream &operator<<(std::ostream &out, const VectorPointBase<T, size> &vp)
{
    out << vp.at(0);

    for (int i = 1; i < size; ++i)
    {
        out << ", " << vp(i);
    }

    return out;
}
} // namespace MathLib

#endif