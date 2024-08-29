#pragma once

#include <iostream>
#include <string>
#include <memory>

template<typename T>
class LinearAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    using is_always_equal = std::false_type;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    template <typename U>
    struct rebind
    {
        using other = LinearAllocator<U>;
    };

    LinearAllocator() = delete;
    
    LinearAllocator(size_t poolSize = 10) :
        m_pool{new char[poolSize * sizeof(T)]},
        m_poolSize{poolSize},
        m_currentPtr{m_pool.get()},
        m_finalPtr{m_pool.get() + poolSize * sizeof(T)}
    {
        std::cout << this << " constructor, sizeof(T): " << sizeof(T) << '\n';
    }

    template<typename U>
    LinearAllocator(const LinearAllocator<U>& other) noexcept :
        m_pool{new char[other.m_poolSize * sizeof(T)]},
        m_poolSize{other.m_poolSize},
        m_currentPtr{m_pool.get()},
        m_finalPtr{m_pool.get() + other.m_poolSize * sizeof(T)}
    {
        std::cout << this << " copy constructor (U), sizeof(T): " << sizeof(T) << '\n';
    }

    LinearAllocator(LinearAllocator&& other) noexcept :
        // m_pool{std::move(other.m_pool)},
        m_poolSize{other.m_poolSize},
        m_currentPtr{other.m_currentPtr},
        m_finalPtr{other.m_finalPtr}
    {
        m_pool.reset();
        m_pool = other.m_pool;
        std::cout << this << " move constructor, sizeof(T): " << sizeof(T) << '\n';
    }

    LinearAllocator& operator=(LinearAllocator&& other) noexcept
    {
        std::cout << this << " move assignment, sizeof(T): " << sizeof(T) << '\n';
        m_pool = std::move(other.m_pool);
        m_poolSize = other.m_poolSize;
        m_currentPtr = other.m_currentPtr;
        m_finalPtr = other.m_finalPtr;
        return *this;
    }

    LinearAllocator(const LinearAllocator& other) noexcept :
        // m_pool{other.m_pool},
        m_poolSize{other.m_poolSize},
        m_currentPtr{other.m_currentPtr},
        m_finalPtr{other.m_finalPtr}
    {
        m_pool.reset();
        m_pool = other.m_pool;
        std::cout << this << " copy constructor, sizeof(T): " << sizeof(T) << '\n';
    }

    LinearAllocator& operator=(const LinearAllocator& other) noexcept
    {
        std::cout << this << " copy assignment, sizeof(T): " << sizeof(T) << '\n';
        m_pool = other.m_pool;
        m_poolSize = other.m_poolSize;
        m_currentPtr = other.m_currentPtr;
        m_finalPtr = other.m_finalPtr;
        return *this;
    }

    pointer allocate(size_type n)
    {
        if(n > m_poolSize)
        {
            throw std::bad_alloc();
        }
        char* returnPtr = m_currentPtr;
        m_currentPtr += n * sizeof(T);
        if(m_currentPtr > m_finalPtr)
        {
            throw std::bad_alloc();
        }
        std::cout << this << " A [" << n << "]: " << (void*)returnPtr << std::endl;
        return reinterpret_cast<T*>(returnPtr);
    }

    void deallocate(pointer ptr, size_type n)
    {
        std::cout << this << " D [" << n << "]: " << ptr << std::endl;
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        std::cout << this << " C [" << 1 << "]: " << p << std::endl;
        new (p) U(std::forward<Args>(args)...);
    };

    template <typename U>
    void destroy(U *p) {
        std::cout << this << " ~ [" << 1 << "]: " << p << std::endl;
        p->~U();
    }

private:
    std::shared_ptr<char[]> m_pool;
    std::size_t m_poolSize;
    char* m_finalPtr;
    char* m_currentPtr;
    template<typename T1, typename T2>
    friend bool operator==(const LinearAllocator<T1> &lhs, const LinearAllocator<T2> &rhs);
    template<typename T1, typename T2>
    friend bool operator!=(const LinearAllocator<T1> &lhs, const LinearAllocator<T2> &rhs);
    template <typename U>
    friend class LinearAllocator;
};

template<typename T1, typename T2>
bool operator==(const LinearAllocator<T1> &lhs, const LinearAllocator<T2> &rhs)
{
    return lhs.m_pool.get() == rhs.m_pool.get();
}

template<typename T1, typename T2>
bool operator!=(const LinearAllocator<T1> &lhs, const LinearAllocator<T2> &rhs)
{
    return lhs.m_pool.get() != rhs.m_pool.get();
}