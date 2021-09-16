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

    template <typename U = T>
    U norm() const
    {
        return sqrt(dot(*this, *this));
    }

    template <typename U = T>
    U norm_squared() const
    {
        return dot(*this, *this);
    }

    // returns the angle between this and the other vector
    double angleTo(const Vector<T, size> &other) const
    {
        return acos(dot(*this, other) / (this->norm() * other.norm()));
    }

    static Vector<T, size> random()
    {
        Vector<T, size> v{};

        for (int i{0}; i < size; ++i)
        {
            v.m_data[i] = Util::random_number<T>();
        }

        return v;
    }

    static Vector<T, size> random(T min, T max)
    {
        Vector<T, size> v{};

        for (int i{0}; i < size; ++i)
        {
            v.m_data[i] = Util::random_number<T>(min, max);
        }

        return v;
    }
};

template <typename T, int size>
Vector<T, size> &operator+=(Vector<T, size> &vector, const Vector<T, size> &other)
{
    for (int i = 0; i < vector.size(); ++i)
    {
        vector(i) += other(i);
    }

    return vector;
}

template <typename T, int size>
Vector<T, size> &operator-=(Vector<T, size> &vector, const Vector<T, size> &other)
{
    for (int i = 0; i < vector.size(); ++i)
    {
        vector(i) -= other(i);
    }

    return vector;
}

template <typename T, int size>
Vector<T, size> operator+(const Vector<T, size> &v1, const Vector<T, size> &v2)
{
    Vector<T, size> sum{v1};

    return sum += v2;
}

template <typename T, int size>
Vector<T, size> operator-(const Vector<T, size> &v1, const Vector<T, size> &v2)
{
    Vector<T, size> diff{v1};

    return diff -= v2;
}

template <typename T, int size, typename U = T>
U dot(const Vector<T, size> &v1, const Vector<T, size> &v2)
{
    U sum{0};

    for (int i = 0; i < size; ++i)
    {
        sum += v1.at(i) * v2.at(i);
    }

    return sum;
}

template <typename T>
Vector<T, 3> cross(const Vector<T, 3> &v1, const Vector<T, 3> &v2)
{
    Vector<T, 3> newVec;

    newVec.at(0) = v1.at(1) * v2.at(2) - v1.at(2) * v2.at(1);
    newVec.at(1) = v1.at(2) * v2.at(0) - v1.at(0) * v2.at(2);
    newVec.at(2) = v1.at(0) * v2.at(1) - v1.at(1) * v2.at(0);

    return newVec;
}

template <typename T, int size>
Vector<T, size> &normalize(Vector<T, size> &vector)
{
    vector /= vector.norm();
    return vector;
}

template <typename T, int size>
Vector<T, size> normalize(const Vector<T, size> &vector)
{
    Vector<T, size> newVector{vector};

    normalize(newVector);

    return newVector;
}

template <typename T, int size>
Vector<T, size> reflect(const Vector<T, size> &vector, const Vector<T, size> &normal)
{
    return vector - 2 * dot(vector, normal) * normal;
}

template <typename T, int size>
Vector<T, size> refract(const Vector<T, size> &vector, const Vector<T, size> &normal, double etaiOverEtat)
{
    auto cosTheta{std::min(dot(-vector, normal), static_cast<T>(1.0))};
    Vector<T, size> rOutPerp{etaiOverEtat * (vector + cosTheta * normal)};
    Vector<T, size> rOutParallel{-std::sqrt(std::abs(1.0 - rOutPerp.norm_squared())) * normal};
    return rOutParallel + rOutPerp;
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