#ifndef MATHLIB_UTIL_UTIL_H
#define MATHLIB_UTIL_UTIL_H

#include <limits>
#include <type_traits>
#include <math.h>

namespace MathLib {
    namespace Util {
        // implementation is based on  the code in this post: https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
        template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
        bool isClose(T a, T b, T maxDiff = std::numeric_limits<T>::epsilon(), T maxRelDiff = std::numeric_limits<T>::epsilon())
        {
            T diff = abs(a - b);
            if (diff < maxDiff)
            {
                return true;
            }

            a = abs(a);
            b = abs(b);
            T largest =(b > a) ? b : a;

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
    }
}

#endif