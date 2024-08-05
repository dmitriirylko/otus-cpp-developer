#pragma once

#include <iostream>

template<typename T>
class LinearAlloctor
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    LinearAlloctor(const std::size_t poolSize) :
        m_poolSize{poolSize},
        m_startPtr{new char[m_poolSize * sizeof(T)]},
        m_finalPtr{m_startPtr + m_poolSize * sizeof(T)},
        m_currentPtr{m_startPtr}
    {
        if(m_currentPtr)
        {
            std::cout   << "Linear allocator constructed. Pool size: "
                        << m_poolSize
                        << ", start address: "
                        << (void*)m_currentPtr
                        << std::endl;
        }
        else
        {
            throw std::bad_alloc();
        }
    }
    // LinearAlloctor(const LinearAlloctor& rhs) :
    //     m_poolSize{rhs.m_poolSize},
    //     m_startPtr{new char[m_poolSize * sizeof(T)]},
    //     m_finalPtr{m_startPtr + m_poolSize * sizeof(T)},
    //     m_currentPtr{const_cast<char*>(m_startPtr)}
    //     {
    //         if(m_currentPtr)
    //         {
    //             std::cout   << "Linear allocator copy constructed. Pool size: "
    //                         << m_poolSize
    //                         << ", start address: "
    //                         << (void*)m_currentPtr
    //                         << std::endl;
    //         }
    //         else
    //         {
    //             throw std::bad_alloc();
    //         }
    //     }
    LinearAlloctor(LinearAlloctor&& rhs) :
        m_poolSize{0},
        m_startPtr{nullptr},
        m_finalPtr{nullptr},
        m_currentPtr{nullptr}
    {
        std::swap(m_poolSize, rhs.m_poolSize);
        std::swap(m_startPtr, rhs.m_startPtr);
        std::swap(m_finalPtr, rhs.m_finalPtr);
        std::swap(m_currentPtr, rhs.m_currentPtr);
        std::cout   << "Linear allocator move constructed" <<std::endl;
    }
    LinearAlloctor& operator=(const LinearAlloctor&) = delete;
    LinearAlloctor& operator=(LinearAlloctor&&) = delete;
    ~LinearAlloctor()
    {
        std::cout << "Linear allocator destructed" << std::endl;
        delete [] m_startPtr;
    }
    T* allocate(std::size_t n)
    {
        if(n > m_poolSize)
        {
            throw std::bad_alloc();
        }
        char* returnPtr = m_currentPtr;
        m_currentPtr += n * sizeof(T);
        if(m_currentPtr >= m_finalPtr)
        {
            throw std::bad_alloc();
        }
        std::cout   << "Allocate: [n = " << n << "], at: "
                    << (void*)returnPtr << std::endl;
        return reinterpret_cast<T*>(returnPtr);
    }
    void deallocate(T *p, std::size_t n)
    {
        std::cout << "deallocate: [n = " << n << "]" << std::endl;
    }

private:
    std::size_t m_poolSize;
    char* m_startPtr;
    char* m_finalPtr;
    char* m_currentPtr;
};