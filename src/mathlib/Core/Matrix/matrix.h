#ifndef MATHLIB_CORE_MATRIX_MATRIX_TEMPLATE
#define MATHLIB_CORE_MATRIX_MATRIX_TEMPLATE

#include <type_traits>
#include <cassert>
#include <iostream>
#include <math.h>
#include "../Vector/vector.h"
#include "../../util/type_traits.h"

namespace MathLib {

    // a template for a basic matrix of static size (data stored in column major order)
    template<typename T, int rows, int cols, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix {
    protected:

        T* m_data = nullptr;
        friend class Matrix<T, rows-1, cols-1>;
        friend class Matrix<T, rows+1, cols+1>;

    public:

        Matrix()
        {
            m_data = new T[rows*cols];
        }

        ~Matrix()
        {
            if (m_data != nullptr) {
                delete [] m_data;
            }
            m_data = nullptr;
        }

        //provide constructor that is only callable with correct number of numerical parameters
        template<typename... Tail>
        Matrix(typename std::enable_if<sizeof...(Tail)+1 == rows*cols && are_arithmetic<T, Tail...>{}, T>::type head, Tail... tail)
            : m_data{ new T[rows*cols] }
        {
            const T tmp[rows*cols]{ head, T(tail)...};

            for(int i = 0; i < rows; ++i) {
                for(int j = 0; j < cols; ++j) {
                    m_data[j*rows+i] = tmp[i*cols+j];
                }
            }
        }

        /**
         * Constructor for initialization with column vectors
         **/
        template<typename... Tail>
        Matrix(typename std::enable_if<sizeof...(Tail)+1 == cols && are_same<Vector<T, rows>, Tail...>{}, Vector<T, rows>>::type head, Tail... tail)
            : m_data{ new T[rows*cols] }
        {
            const Vector<T, rows> tmp[cols]{ head, tail...};

            int index = 0;
            for(int i = 0; i < cols; ++i) {
                for(int j = 0; j < rows; ++j) {
                    m_data[index] = tmp[i].at(j);
                    ++index;
                }
            }
        }
        
        Matrix(const Matrix &other) : Matrix()
        {
            *this = other;
        }

        Matrix(Matrix && other) {
            *this = other;
        }

        Matrix& operator=(const Matrix &other)
        {
            if (this == &other)
            {
                return *this;
            }

            for(int i = 0; i < rows*cols; ++i)
            {
                m_data[i] = other.m_data[i];
            }

            return *this;
        }

        Matrix& operator=(Matrix && other) {
            this->m_data = other.m_data;
            other.m_data = nullptr;

            return *this;
        }

        // copy construction and assignment with conversion
        template<typename U>
        Matrix(const Matrix<U, rows, cols> &other) : Matrix()
        {
            *this = other;
        }

        template <typename U>
        Matrix<T, rows, cols>& operator=(const Matrix<U, rows, cols> &other)
        {
            const U* raw = other.raw();
            for(int i = 0; i < rows*cols; ++i)
            {
                m_data[i] = raw[i];
            }

            return *this;
        }

        //create Matrix equal to the upper left subMatrix of the given Matrix
        Matrix(const Matrix<T, rows+1, cols+1> &other) : Matrix()
        {
            for(int i = 0; i < cols; ++i)
            {
                for(int j = 0; j < rows; ++j)
                {
                    m_data[i*rows+j] = other.m_data[i*(rows+1)+j];
                }
            }
        }

        /**
         * Insert matrix into a new matrix with one additional row and col
         * Final layout for Matrix<T, row+1, col+1>{A}
         * 
         * A 0
         * 0 1
         **/
        Matrix(const Matrix<T, rows-1, cols-1> &other) : Matrix()
        {
            for(int i = 0; i < (cols-1); ++i)
            {
                for(int j = 0; j < (rows-1); ++j)
                {
                    m_data[i*rows+j] = other.m_data[i*(rows-1)+j];
                }
            }

            int lastColBase = rows*(cols-1);
            for(int i = 0; i < (rows-1); ++i)
            {
                m_data[lastColBase+i] = 0;
            }

            int lastRowBase = rows-1;
            for(int i = 0; i < (cols-1); ++i)
            {
                m_data[i*rows+lastRowBase] = 0;
            }

            m_data[cols*rows-1] = 1;
        }

        Matrix<T, rows, cols>& negate()
        {
            return *this *= -1;
        }

        Matrix<T, rows, cols>& transpose()
        {
            assert(rows == cols);

            for(int col = 1; col < cols; ++col){
                for(int row = 0; row < col; ++row){
                    T temp = m_data[col * rows + row];
                    m_data[col * rows + row] = m_data[row * cols + col];
                    m_data[row * cols + col] = temp;
                }
            }

            return *this;
        }
        
        Matrix<T, rows, cols>& setIdentity()
        {
            assert("Using setIdentity on a matrix that is not square" && rows == cols);

            for(int i{0}; i < rows; ++i)
            {
                for(int j{0}; j < cols; ++j)
                {
                    m_data[j*rows+i] = (i == j) ? 1 : 0;
                }
            }

            return *this;
        }

        T operator()(int row, int col) const
        {
            return m_data[col*rows+row];
        }

        T& operator()(int row, int col)
        {
            return m_data[col*rows+row];
        }

        T at(int row, int col) const
        {
            assert("Accessing matrix with index out of its bounds" && row >= 0 && row < rows && col >= 0 && col < cols);

            return m_data[col*rows+row];
        }

        T& at(int row, int col)
        {
            assert("Accessing matrix with index out of its bounds" && row >= 0 && row < rows && col >= 0 && col < cols);

            return m_data[col*rows+row];
        }

        void set(int row, int col, T val)
        {
            assert("Accessing matrix with index out of its bounds" && row >= 0 && row < rows && col >= 0 && col < cols);

            m_data[col*rows+row] = val;
        }

        // returns the internal array (BEWARE!: the internal storage is in row major order)
        T* raw() const {
            return m_data;
        }

        Matrix<T, rows, cols>& operator+=(const Matrix<T, rows, cols> &other)
        {
            for (int i = 0; i < rows*cols; ++i)
            {
                m_data[i] += other.m_data[i];
            }

            return *this;
        }

        Matrix<T, rows, cols>& operator-=(const Matrix<T, rows, cols> &other)
        {
            for (int i = 0; i < rows*cols; ++i)
            {
                m_data[i] -= other.m_data[i];
            }

            return *this;
        }

        template<typename V>
        Matrix<T, rows, cols>& operator*=(V scalar)
        {
            for (int i = 0; i < rows*cols; ++i)
            {
                m_data[i] *= scalar;
            }

            return *this;
        }

        template<typename V>
        Matrix<T, rows, cols>& operator/=(V scalar)
        {
            assert("Division by zero" && scalar != 0);

            for (int i = 0; i < rows*cols; ++i)
            {
                m_data[i] /= scalar;
            }

            return *this;
        }

        T trace() const{
            assert(rows == cols);

            T sum{ 0 };

            for(int i{ 0 }; i < rows; ++i){
                sum += m_data[i*rows+i];
            }

            return sum;
        }
    };

    template <typename T, int rows, int cols>
    Matrix<T, cols, rows> transpose(const Matrix<T, rows, cols> &mat){
        Matrix<T, cols, rows> trans;

        for(int col = 0; col < cols; ++col){
            for(int row = 0; row < rows; ++row){
                trans.at(col, row) = mat.at(row, col);
            }
        }

        return trans;
    }

    template<typename T, int rows, int cols>
    Matrix<T, rows, cols> operator+(const Matrix<T, rows, cols> &m1, const Matrix<T, rows, cols> &m2)
    {
        Matrix<T, rows, cols> sum{ m1 };

        return sum += m2;
    }

    template<typename T, int rows, int cols>
    Matrix<T, rows, cols> operator-(const Matrix<T, rows, cols> &m1, const Matrix<T, rows, cols> &m2)
    {
        Matrix<T, rows, cols> diff{ m1 };

        return diff -= m2;
    }

    template<typename T, int rows, int cols, typename V>
    Matrix<T, rows, cols> operator*(const Matrix<T, rows, cols> &m1, V scalar)
    {
        Matrix<T, rows, cols> base{ m1 };

        return base *= scalar;
    }

    template<typename T, int rows, int cols, typename V>
    Matrix<T, rows, cols> operator*(V scalar, const Matrix<T, rows, cols> &m1)
    {
        Matrix<T, rows, cols> base{ m1 };

        return base *= scalar;
    }

    template<typename T, int rowsM1, int colsM1rowsM2, int colsM2, typename V>
    Matrix<T, rowsM1, colsM2> operator*(const Matrix<T, rowsM1, colsM1rowsM2> &m1, const Matrix<V, colsM1rowsM2, colsM2> &m2){
        Matrix<T, rowsM1, colsM2> res;

        for(int col{ 0 }; col < colsM2; ++col){
            for(int row{ 0 }; row < rowsM1; ++row){
                T sum{ 0 };
                for(int i{ 0 }; i < colsM1rowsM2; ++i){
                    sum += m1.at(row, i) * m2.at(i, col);
                }
                res.set(row, col, sum);
            }
        }

        return res;
    }

    template<typename T, int rows, int cols, typename V>
    Vector<T, rows> operator*(const Matrix<T, rows, cols> &mat, const Vector<V, cols> &vec)
    {
        Vector<T, rows> res{};

        for (int i{0}; i < rows; ++i)
        {
            T sum{0};
            for(int j{0}; j < cols; ++j)
            {
                sum += mat.at(i, j) * vec.at(j);
            }
            res.set(i, sum);
        }

        return res;
    }

    template<typename T, int rows, int cols, typename V>
    Matrix<T, rows, cols> operator/(const Matrix<T, rows, cols> &m1, V scalar)
    {
        Matrix<T, rows, cols> base{ m1 };

        return base /= scalar;
    }

    template<typename T, int rows, int cols>
    Matrix<T, rows, cols> operator-(const Matrix<T, rows, cols> &mat)
    {
        Matrix<T, rows, cols> negation{ mat };

        return negation.negate();
    }

    template<typename T, int rows, int cols>
    bool operator==(const Matrix<T, rows, cols> &m1, const Matrix<T, rows, cols> &m2)
    {
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                if (m1(i, j) != m2(i, j))
                {
                    return false;
                }
            }
        }

        return true;
    }

    template<typename T, int rows, int cols, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    bool allClose(const Matrix<T, rows, cols> &m1, const Matrix<T, rows, cols> &m2, T maxDiff = std::numeric_limits<T>::epsilon(), T maxRelDiff = std::numeric_limits<T>::epsilon()) {
        for (int i = 0; i < rows; ++i){
            for (int j = 0; j < cols; ++j) {
                if (!Util::isClose(m1(i, j), m2(i, j), maxDiff, maxRelDiff)) {
                    return false;
                }
            }
        }

        return true;
    }

    template<typename T, int rows, int cols>
    bool operator!=(const Matrix<T, rows, cols> &m1, const Matrix<T, rows, cols> &m2)
    {
        return !(m1 == m2);
    }

    template <typename T, int rows, int cols>
    std::ostream& operator<< (std::ostream &out, Matrix<T, rows, cols> &mat){
        out << "[ ";

        for(int i = 0; i < rows; ++i){
            if(i == 0){
                out << "[ " << mat.at(i, 0);
            }else{
                out << ", [ " << mat.at(i, 0);
            }
            for(int j = 1; j < cols; ++j){
                out << ", " << mat.at(i, j);
            }
            out << " ]";
        }

        out << " ]";

        return out;
    }

    template<typename T>
    Matrix<T, 4, 4> getTranslation(const Vector<T, 3>& translation) {
        return Matrix<T, 4, 4>{
            1, 0, 0, translation(0),
            0, 1, 0, translation(1),
            0, 0, 1, translation(2),
            0, 0, 0, 1
        };
    }

    template<typename T>
    Matrix<T, 3, 3> getScaling(const Vector<T, 3>& scaling) {
        return Matrix<T, 3, 3>{
            scaling(0), 0,          0,
            0,          scaling(1), 0,
            0,          0,          scaling(2)
        };
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    Matrix<T, 3, 3> getRotateX(T rad) {
        return Matrix<T, 3, 3>{
            1.0,      0.0,       0.0,
            0.0, cos(rad), -sin(rad),
            0.0, sin(rad),  cos(rad)
        };
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    Matrix<T, 3, 3> getRotateY(T rad) {
        return Matrix<T, 3, 3>{
             cos(rad), 0.0, sin(rad),
                  0.0, 1.0,      0.0,
            -sin(rad), 0.0, cos(rad)
        };
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    Matrix<T, 3, 3> getRotateZ(T rad) {
        return Matrix<T, 3, 3>{
            cos(rad), -sin(rad), 0.0,
            sin(rad),  cos(rad), 0.0,
                 0.0,       0.0, 1.0
        };
    }

    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
    Matrix<T, 3, 3> getRotation(const Vector<T, 3>& rotation) {
        return getRotateX(rotation(0)) * getRotateY(rotation(1)) * getRotateZ(rotation(2));
    }
}
#endif