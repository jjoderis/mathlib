#ifndef MATHLIB_CORE_QUATERNION_QUATERNION_TEMPLATE
#define MATHLIB_CORE_QUATERNION_QUATERNION_TEMPLATE

#include <type_traits>
#include "../Vector/vector.h"

namespace MathLib {

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    class Quaternion {
    protected:
        Vector<T, 3> m_qv{};
        T m_qw{};

    public:
        Quaternion() {}

        template<typename U, typename V, typename = typename std::enable_if<std::is_floating_point<U>::value, U>::type, typename = typename std::enable_if<std::is_floating_point<V>::value, V>::type>
        Quaternion(const Vector<U, 3>& imaginary, V real) {
            m_qv = imaginary;
            m_qw = real;
        }

        template<typename U, typename = typename std::enable_if<std::is_floating_point<U>::value, U>::type>
        Quaternion(U x, U y, U z, U w) : Quaternion{}
        {
            m_qv(0) = x;
            m_qv(1) = y;
            m_qv(2) = z;
            m_qw = w;
        }

        Quaternion(const Quaternion& other) : Quaternion{} {
            m_qv = other.m_qv;
            m_qw = other.m_qw;
        }

        Quaternion& operator=(const Quaternion& other) {

            if (&other == this) {
                return *this;
            }

            m_qv = other.m_qv;
            m_qw = other.m_qw;

            return *this;
        }

        template <typename U>
        Quaternion(const Quaternion<U>& other) : Quaternion{} {
            m_qv = other.qv();
            m_qw = other.qw();
        }

        T& qx() {
            return m_qv(0);
        }

        const T& qx() const {
            return m_qv(0);
        }

        T& qy() {
            return m_qv(1);
        }

        const T& qy() const {
            return m_qv(1);
        }

        T& qz() {
            return m_qv(2);
        }

        const T& qz() const {
            return m_qv(2);
        }

        T& qw() {
            return m_qw;
        }

        const T& qw() const {
            return m_qw;
        }

        Vector<T, 3>& qv() {
            return m_qv;
        }

        const Vector<T, 3>& qv() const {
            return m_qv;
        }

        Quaternion getConjugate() const {
            return Quaternion{-m_qv, m_qw};
        }

        T norm() const {
            return sqrt(pow(m_qv(0), 2.0) + pow(m_qv(1), 2.0) + pow(m_qv(2), 2.0) + pow(m_qw, 2.0));
        }

        Quaternion& setIdentity() {
            m_qv = Vector<T, 3>{0.0, 0.0, 0.0};
            m_qw = 1;

            return *this;
        }

        Quaternion getInverse() const {
            return (1 / pow(this->norm(), 2.0)) * this->getConjugate();
        }

        Quaternion& setUnit() {
            *this = (1/this->norm()) * (*this);

            return *this;
        }

        Quaternion getUnit() {
            Quaternion unit{*this};
            unit.setUnit();
            return unit;
        }

        Quaternion& setRotation(const Vector<T, 3>& axis, T angle) {
            Vector<T, 3> normalizedAxis{axis};
            normalizedAxis.normalize();
            m_qv = normalizedAxis * sin(angle/2);
            m_qw = cos(angle/2);
            return *this;
        }
    };

    template<typename T, typename U>
    Quaternion<T> operator*(const Quaternion<T>& q, const Quaternion<U>& r) {
        Quaternion<T> product{};

        product.qv() = (cross(q.qv(), r.qv()) + r.qw() * q.qv() + q.qw() * r.qv());
        product.qw() = q.qw() * r.qw() - dot(q.qv(), r.qv());

        return product;
    }

    template<typename T, typename U>
    Quaternion<T> operator*(const Quaternion<T>& q, U scalar) {
        Quaternion<T> product{};

        product.qv() = q.qv() * scalar;
        product.qw() = q.qw() * scalar;

        return product;
    }

    template<typename T, typename U>
    Quaternion<U> operator*(T scalar, const Quaternion<U>& q) {
        Quaternion<U> product{};

        product.qv() = q.qv() * scalar;
        product.qw() = q.qw() * scalar;

        return product;
    }

    template<typename T, typename U>
    Quaternion<T> operator+(const Quaternion<T>& q, const Quaternion<U>& r) {
        Quaternion<T> sum{};

        sum.qv() = q.qv() + r.qv();
        sum.qw() = q.qw() + r.qw();

        return sum;
    }

    template<typename T>
    Quaternion<T> slerp(const Quaternion<T>& q, const Quaternion<T>& r, T stepSize) {
        T angle{acos(dot(q.qv(), r.qv()) + q.qw() * r.qw())};

        return (sin(angle * (1-stepSize))/sin(angle)) * q + (sin(angle*stepSize)/sin(angle)) * r;
    }

    template<typename T>
    bool operator==(const Quaternion<T>& q1, const Quaternion<T>& q2) {
        return allClose(q1.qv(), q2.qv()) && Util::isClose(q1.qw(), q2.qw());
    }
}

#endif