#ifndef MATHLIB_CORE_VECTOR_VECTOR_TEMPLATE
#define MATHLIB_CORE_VECTOR_VECTOR_TEMPLATE

#include "../../util/type_traits.h"
#include "../../util/util.h"
#include "./vectorPointBase.h"
#include <cassert>
#include <iostream>
#include <math.h>
#include <type_traits>

namespace MathLib
{
// A template for a basic vector of static size
template <typename T, int size, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Vector : public VectorPointBase<T, size>
{
public:
    Vector() = default;

    // provide constructor that is only callable with correct number of parameters
    template <typename... Tail>
    Vector(T head, Tail... tail) : VectorPointBase<T, size>{head, T(tail)...}
    {
    }

    Vector(const Vector<T, size - 1> &other, T val) : VectorPointBase<T, size>{other, val} {}

    Vector(const Vector<T, size + 1> &other) : VectorPointBase<T, size>{other} {}

    double norm() const { return sqrt(dot(*this, *this)); }

    // returns the angle between this and the other vector
    double angleTo(const Vector<T, size> &other) const
    {
        return acos(dot(*this, other) / (this->norm() * other.norm()));
    }
};

std::false_type is_vector_impl(...);
template <typename T, int size>
std::true_type is_vector_impl(Vector<T, size> const volatile &);

template <typename T>
using is_vector = decltype(is_vector_impl(std::declval<T &>()));

template <typename T>
typename std::enable_if<is_vector<T>::value, T>::type &operator+=(T &vector, const T &other)
{
    for (int i = 0; i < vector.size(); ++i)
    {
        vector(i) += other(i);
    }

    return vector;
}

template <typename T>
typename std::enable_if<is_vector<T>::value, T>::type &operator-=(T &vector, const T &other)
{
    for (int i = 0; i < vector.size(); ++i)
    {
        vector(i) -= other(i);
    }

    return vector;
}

template <typename T>
typename std::enable_if<is_vector<T>::value, T>::type operator+(const T &v1, const T &v2)
{
    T sum{v1};

    return sum += v2;
}

template <typename T>
typename std::enable_if<is_vector<T>::value, T>::type operator-(const T &v1, const T &v2)
{
    T diff{v1};

    return diff -= v2;
}

template <typename T, int size>
double dot(const Vector<T, size> &v1, const Vector<T, size> &v2)
{
    double sum{0};

    for (int i = 0; i < size; ++i)
    {
        sum += v1.at(i) * v2.at(i);
    }

    return sum;
}

std::false_type is_vector3_impl(...);
template <typename T>
std::true_type is_vector3_impl(Vector<T, 3> const volatile &);

template <typename T>
using is_vector3 = decltype(is_vector3_impl(std::declval<T &>()));

template <typename T>
typename std::enable_if<is_vector3<T>::value, T>::type cross(const T &v1, const T &v2)
{
    T newVec;

    newVec.at(0) = v1.at(1) * v2.at(2) - v1.at(2) * v2.at(1);
    newVec.at(1) = v1.at(2) * v2.at(0) - v1.at(0) * v2.at(2);
    newVec.at(2) = v1.at(0) * v2.at(1) - v1.at(1) * v2.at(0);

    return newVec;
}

template <typename T>
typename std::enable_if<is_vector<T>::value, T>::type &normalize(T &vector)
{
    vector /= vector.norm();
    return vector;
}

template <typename T>
typename std::enable_if<is_vector<T>::value, T>::type normalize(const T &vector)
{
    T newVector{vector};

    normalize(newVector);

    return newVector;
}

template <typename T>
typename std::enable_if<is_vector<T>::value, T>::type reflect(const T &vector, const T &normal)
{
    return vector - 2 * dot(vector, normal) * normal;
}

template <typename T, int size>
bool operator==(const Vector<T, size> &v1, const Vector<T, size> &v2)
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
bool allClose(const Vector<T, size> &v1,
              const Vector<T, size> &v2,
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
bool operator!=(const Vector<T, size> &v1, const Vector<T, size> &v2)
{
    return !(v1 == v2);
}

template <typename T, int size>
std::ostream &operator<<(std::ostream &out, const Vector<T, size> &vector)
{
    out << "[ ";

    out << (VectorPointBase<T, size>)vector;

    out << " ]";

    return out;
}
} // namespace MathLib

#endif