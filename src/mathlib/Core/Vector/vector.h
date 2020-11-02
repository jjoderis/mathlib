#ifndef MATHLIB_CORE_VECTOR_VECTOR_TEMPLATE
#define MATHLIB_CORE_VECTOR_VECTOR_TEMPLATE

#include <type_traits>
#include <cassert>
#include <iostream>
#include <math.h>
#include <util/type_traits.h>

namespace Core{
    //A template for a basic vector of static size
    template<typename T, int size, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Vector{
    protected:

        T* m_data = nullptr;
        friend class Vector<T, size-1>;
        friend class Vector<T, size+1>;

    public: 
        Vector()
        {
            m_data = new T[size];
        }

        ~Vector()
        {
            delete [] m_data;
            m_data = nullptr;
        }
        
        //provide constructor that is only callable with correct number of parameters
        template<typename... Tail>
        Vector(typename std::enable_if<sizeof...(Tail)+1 == size && are_arithmetic<Tail...>{}, T>::type head, Tail... tail)
            : m_data{ new T[size]{ head, T(tail)...} }{}

        Vector(const Vector &other) : Vector()
        {
            *this = other;
        }

        Vector<T, size>& operator=(const Vector<T, size> &other)
        {
            if(this == &other)
            {
                return *this;
            }

            for(int i = 0; i < size; ++i)
            {
                m_data[i] = other.m_data[i];
            }

            return *this;
        }

        // copy construction and assignment with conversion
        template<typename U>
        Vector(const Vector<U, size> &other) : Vector()
        {
            *this = other;
        }

        template <typename U>
        Vector<T, size>& operator=(const Vector<U, size> &other)
        {
            for(int i = 0; i < size; ++i)
            {
                m_data[i] = other.at(i);
            }

            return *this;
        }

        Vector(const Vector<T, size-1> &other, T val) : Vector()
        {
            for(int i = 0; i  < size; ++i)
            {
                m_data[i] = other.m_data[i];
            }

            m_data[size-1] = val;
        }

        Vector(const Vector<T, size+1> &other) : Vector()
        {
            for(int i = 0; i  < size; ++i)
            {
                m_data[i] = other.m_data[i];
            }
        }

        // using () operator for subscript to have same API as matrix ([] can't take two arguments)
        T operator()(int index) const
        {
            return m_data[index];
        }

        T& operator()(int index)
        {
            return m_data[index];
        }

        T at(int index) const
        {
            assert("Accessing vector with index out of its bounds" && index >= 0 && index < size);

            return m_data[index];
        }

        T& at(int index)
        {
            assert("Accessing vector with index out of its bounds" && index >= 0 && index < size);

            return m_data[index];
        }

        void set(int index, T val)
        {
            assert("Accessing vector with index out of its bounds" && index >= 0 && index < size);

            m_data[index] = val;
        }

        T* raw() const
        {
            return m_data;
        }

        Vector<T, size>& operator+=(const Vector<T, size> &other)
        {
            for(int i = 0; i < size; ++i)
            {
                m_data[i] += other.m_data[i];
            }

            return *this;
        }

        Vector<T, size>& operator-=(const Vector<T, size> &other)
        {
            for(int i = 0; i < size; ++i)
            {
                m_data[i] -= other.m_data[i];
            }

            return *this;
        }

        template<typename V>
        Vector<T, size>& operator*=(const V val)
        {
            for(int i = 0; i < size; ++i)
            {
                m_data[i] *= val;
            }

            return *this;
        }

        template<typename V>
        Vector<T, size>& operator/=(const V val)
        {
            for(int i = 0; i < size; ++i)
            {
                m_data[i] /= val;
            }

            return *this;
        }

        Vector<T, size>& negate()
        {
            for(int i = 0; i < size; ++i)
            {
                m_data[i] *= -1;
            }

            return *this;
        }

        float norm() const
        {
            return sqrt(dot(*this, *this));
        }

        Vector<T, size>& normalize()
        {
            *this /= this->norm();
            return *this;
        }

        //returns the angle between this and the other vector
        double angleTo(const Vector<T, size> &other) const{
            return acos(dot(*this, other)/(this->norm()*other.norm()));
        }
    };

    template<typename T, int size>
    Vector<T, size> operator+(const Vector<T, size> &v1, const Vector<T, size> &v2)
    {
        Vector<T, size> sum{ v1 };

        return sum += v2;
    }

    template<typename T, int size>
    Vector<T, size> operator-(const Vector<T, size> &v1, const Vector<T, size> &v2)
    {
        Vector<T, size> diff{ v1 };

        return diff -= v2;
    }

    template<typename T, int size, typename V>
    Vector<T, size> operator*(V val, const Vector<T, size> &vector)
    {
        Vector<T, size> product{ vector };

        return product *= val;
    }

    template<typename T, int size, typename V>
    Vector<T, size> operator*(const Vector<T, size> &vector, V val)
    {
        Vector<T, size> product{ vector };

        return product *= val;
    }

    template<typename T, int size>
    float operator*(const Vector<T, size> &v1, const Vector<T, size> &v2)
    {
        float sum{ 0 };

        for(int i = 0; i < size; ++i)
        {
            sum += v1.at(i) * v2.at(i);
        }

        return sum;
    }

    template<typename T, int size, typename V>
    Vector<T, size> operator/(const Vector<T, size> &vector, V val)
    {
        Vector<T, size> product{ vector };

        return product /= val;
    }
    
    template<typename T, int size>
    Vector<T, size> operator-(const Vector<T, size> &vector)
    {
        Vector<T, size> negation{ vector };

        return negation.negate();
    }

    template<typename T, int size>
    float dot(const Vector<T, size> &v1, const Vector<T, size> &v2)
    {
        return v1 * v2;
    }

    template<typename T>
    Vector<T, 3> cross(const Vector<T, 3> &v1, const Vector<T, 3> &v2){
        Vector<T, 3> newVec;

        newVec.at(0) = v1.at(1) * v2.at(2) - v1.at(2) * v2.at(1);
        newVec.at(1) = v1.at(2) * v2.at(0) - v1.at(0) * v2.at(2);
        newVec.at(2) = v1.at(0) * v2.at(1) - v1.at(1) * v2.at(0);

        return newVec;
    }

    template<typename T, int size>
    bool operator==(const Vector<T, size> &v1, const Vector<T, size> &v2)
    {
        for(int i = 0; i < size; ++i)
        {
            if(v1(i) != v2(i))
            {
                return false;
            }
        }

        return true;
    }

    template<typename T, int size>
    bool operator!=(const Vector<T, size> &v1, const Vector<T, size> &v2)
    {
        return !(v1 == v2);
    }

    template <typename T, int size>
    std::ostream& operator<< (std::ostream &out, const Vector<T, size> &vector){
        out << "[ " << vector.at(0);

        for(int i = 1; i < size; ++i){
            out << ", " << vector.at(i);
        }

        out << " ]";

        return out;
    }
}

#endif