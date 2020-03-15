#ifndef LINALG_VECTOR_VECTOR_TEMPLATE
#define LINALG_VECTOR_VECTOR_TEMPLATE

#include <initializer_list>

namespace LinAlg{
    //A template for a basic vector of static size
    template<typename T, int size>
    class Vector{
    protected:

        T* m_data = nullptr;

    public: 

        Vector()
        {
            m_data = new T[size];
        }

        ~Vector()
        {
            delete [] m_data;
        }   
    };

}

#endif