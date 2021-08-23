#ifndef MATHLIB_CORE_POINT_TEMPLATE
#define MATHLIB_CORE_POINT_TEMPLATE

#include "../../util/type_traits.h"
#include "../../util/util.h"
#include "./vector.h"
#include "./vectorPointBase.h"
#include <cassert>
#include <iostream>
#include <math.h>
#include <type_traits>

namespace MathLib
{
// A template for a basic point of static size
template <typename T, int size, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Point : public VectorPointBase<T, size>
{
public:
    // friend class Vector<T, size - 1>;
    // friend class Vector<T, size + 1>;

    Point() : VectorPointBase<T, size>{} {}

    // provide constructor that is only callable with correct number of parameters
    template <typename... Tail>
    Point(typename std::enable_if<sizeof...(Tail) + 1 == size && are_arithmetic<Tail...>{}, T>::type head, Tail... tail)
        : VectorPointBase<T, size>{head, T(tail)...}
    {
    }

    Point(const Point &other) : VectorPointBase<T, size>{other} {}

    Point<T, size> &operator=(const Point<T, size> &other)
    {
        if (this == &other)
        {
            return *this;
        }

        VectorPointBase<T, size>::operator=(other);

        return *this;
    }

    // copy construction and assignment with conversion
    template <typename U>
    Point(const Point<U, size> &other) : VectorPointBase<T, size>{other}
    {
    }

    template <typename U>
    Point<T, size> &operator=(const Point<U, size> &other)
    {
        VectorPointBase<T, size>::operator=(other);

        return *this;
    }

    // move construction
    Point(Point &&other) : VectorPointBase<T, size>{std::move(other)} {}

    Point &operator=(Point &&other)
    {
        VectorPointBase<T, size>::operator=(std::move(other));

        return *this;
    }

    Point(const Point<T, size - 1> &other, T val) : VectorPointBase<T, size>{other, val} {}

    Point(const Point<T, size + 1> &other) : VectorPointBase<T, size>{other} {}

    Point<T, size> &operator+=(const Vector<T, size> &vector)
    {
        for (int i = 0; i < size; ++i)
        {
            this->m_data[i] += vector(i);
        }

        return *this;
    }

    Point<T, size> &operator-=(const Vector<T, size> &vector)
    {
        for (int i = 0; i < size; ++i)
        {
            this->m_data[i] -= vector(i);
        }

        return *this;
    }
};

template <typename T, int size>
Point<T, size> operator+(const Vector<T, size> &v, const Point<T, size> &p)
{
    return p + v;
}

template <typename T, int size>
Point<T, size> operator+(const Point<T, size> &p, const Vector<T, size> &v)
{
    Point<T, size> sum{p};

    return sum += v;
}

template <typename T, int size>
Point<T, size> operator-(const Point<T, size> &p, const Vector<T, size> &v)
{
    Vector<T, size> diff{p};

    return diff -= v;
}

template <typename T, int size>
Vector<T, size> operator-(const Point<T, size> &p1, const Point<T, size> &p2)
{
    Vector<T, size> diff{};

    for (int i{0}; i < size; ++i)
    {
        diff(i) = p1(i) - p2(i);
    }

    return diff;
}

template <typename T, int size>
std::ostream &operator<<(std::ostream &out, const Point<T, size> &point)
{
    out << "( ";

    out << VectorPointBase<T, size>(point);

    out << " )";

    return out;
}

} // namespace MathLib

#endif