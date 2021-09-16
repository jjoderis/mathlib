#ifndef MATHLIB_UTIL_UTIL_H
#define MATHLIB_UTIL_UTIL_H

#include <limits>
#include <math.h>
#include <random>
#include <type_traits>

namespace MathLib
{
namespace Util
{
// implementation is based on  the code in this post:
// https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
template <typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
bool isClose(T a, T b, T maxDiff = std::numeric_limits<T>::epsilon(), T maxRelDiff = std::numeric_limits<T>::epsilon())
{
    T diff = abs(a - b);
    if (diff < maxDiff)
    {
        return true;
    }

    a = abs(a);
    b = abs(b);
    T largest = (b > a) ? b : a;

    return diff <= (largest * maxRelDiff);
}

template <typename T>
T degToRad(T deg)
{
    return (deg * M_PI) / 180;
}

template <typename T>
T radToDeg(T rad)
{
    return (rad * 180) / M_PI;
}

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
T clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

// Code from Ray Tracing in One Weekend, made into templates to support float as well as double:
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

// returns a random floating point number in [0, 1)
template <typename T>
T random_number()
{
    static std::uniform_real_distribution<T> distribution(0, 1);
    static std::mt19937 generator;
    return distribution(generator);
}

// returns a random floating point number in [min, max)
template <typename T>
T random_number(T min, T max)
{
    return min + (max - min) * random_number<T>();
}
} // namespace Util
} // namespace MathLib

#endif