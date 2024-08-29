#pragma once

#include <stack>
#include <queue>
#include <memory>

// #define LOGGING

class Pool
{
public:
    /**
     * @brief Pool constructor.
     * @param typeSize Size of data structure which is going to be allocated in pool.
     * @param elementsNum Number of elements pool have to store in one memory block.
     */
    explicit Pool(std::size_t typeSize, std::size_t elementsNum = 10) :
        m_typeSize{typeSize},
        m_elementsNum{elementsNum}
    {
        addAddrs();
    }
    /**
     * @brief Gets pointer out of the pool for further allocation. If there are no
     *          vacant addresses in pool it tries to allocate new memory block.
     * @return Pointer out of the pool for allocation.
     */
    void* allocate()
    {
        uint8_t* currentPtrCopy = m_currentPtr;
        m_currentPtr += m_typeSize;
        if(m_currentPtr > m_finalPtr)
        {
            throw std::bad_alloc();
        }
        #ifdef LOGGING
        std::cout << (void*)currentPtrCopy << std::endl;
        #endif
        return currentPtrCopy;
    }
    /**
     * @brief Returns deallocated pointer to addresses queue.
     */
    void deallocate(void *ptr)
    {}
    /**
     * @brief Changes size of data structure which is going to be allocated in pool.
     *          Rebinding after any allocation is unsupported.
     * @param Size of data structure which is going to be allocated in pool.
     */
    void rebind(std::size_t typeSize)
    {
        m_typeSize = typeSize;
    }
    void addAddrs()
    {
        m_block = std::make_unique<uint8_t[]>(m_typeSize * m_elementsNum);
        m_currentPtr = m_block.get();
        m_finalPtr = m_block.get() + m_elementsNum * m_typeSize;
        #ifdef LOGGING
        std::cout << " add_addrs ";
        #endif
    }

private:
    /**
     * @brief Size of data structure which is going to be allocated in pool.
     */
    std::size_t m_typeSize;
    /**
     * @brief Number of elements pool have to store in one memory block.
     */
    size_t m_elementsNum;
    /**
     * @brief Queue of vacant addresses which can be used in allocation.
     */
    // std::queue<void*> m_addrs{};
    /**
     * @brief Stack of memory blocks. Block are added when previous
     *          one has been run out or there are no blocks in stack.
     */
    // std::stack<std::unique_ptr<uint8_t[]>> m_blocks{};
    std::unique_ptr<uint8_t[]> m_block;
    uint8_t* m_currentPtr = nullptr;
    uint8_t* m_finalPtr = nullptr;
};

template<typename T, size_t elementsNum = 10>
class PoolAllocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    using is_always_equal = std::false_type;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    template <typename U>
    struct rebind
    {
        using other = PoolAllocator<U, elementsNum>;
    };
    PoolAllocator() : m_pool{std::make_shared<Pool>(sizeof(T), elementsNum)}
    {
        #ifdef LOGGING
        std::cout << this << " constructor, sizeof(T): " << sizeof(T) << '\n';
        #endif
    }
    template<typename U>
    PoolAllocator(const PoolAllocator<U, elementsNum>& other) noexcept : m_pool{other.m_pool}
    {
        #ifdef LOGGING
        std::cout << this << " copy constructor (U), sizeof(T): " << sizeof(T) << '\n';
        #endif
        m_pool->rebind(sizeof(T));
    }
    PoolAllocator(PoolAllocator&& other) noexcept : m_pool{std::move(other.m_pool)}
    {
        #ifdef LOGGING
        std::cout << this << " move constructor, sizeof(T): " << sizeof(T) << '\n';
        #endif
    }
    PoolAllocator& operator=(PoolAllocator&& other) noexcept
    {
        m_pool = std::move(other.m_pool);
        #ifdef LOGGING
        std::cout << this << " move assignment, sizeof(T): " << sizeof(T) << '\n';
        #endif
        return *this;
    }
    PoolAllocator(const PoolAllocator& other) noexcept : m_pool{other.m_pool}
    {
        #ifdef LOGGING
        std::cout << this << " copy constructor, sizeof(T): " << sizeof(T) << '\n';
        #endif
    }
    PoolAllocator& operator=(const PoolAllocator& other) noexcept
    {
        m_pool = other.m_pool;
        #ifdef LOGGING
        std::cout << this << " copy assignment, sizeof(T): " << sizeof(T) << '\n';
        #endif
        return *this;
    }
    pointer allocate(size_type n)
    {
        #ifdef LOGGING
        std::cout << this << " A [" << n << "]: ";
        #endif
        /* In case of allocation of more than one element our custom allocator works as std::allocator. */
        if(n != 1)
        {
            return static_cast<pointer>(malloc(sizeof(T) * n));
        }
        return static_cast<pointer>(m_pool->allocate());
    }
    /* In case of deallocation of more than one element our custom allocator works as std::allocator. */
    void deallocate(pointer ptr, size_type n)
    {
        #ifdef LOGGING
        std::cout << this << " D [" << n << "]: " << ptr << std::endl;
        #endif
        if(n != 1)
        {
            free(ptr);
        }
        m_pool->deallocate(ptr);
    }
    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        #ifdef LOGGING
        std::cout << this << " C [" << 1 << "]: " << p << std::endl;
        #endif
        new (p) U(std::forward<Args>(args)...);
    };

    template <typename U>
    void destroy(U *p)
    {
        #ifdef LOGGING
        std::cout << this << " ~ [" << 1 << "]: " << p << std::endl;
        #endif
        p->~U();
    }

    bool operator==(const PoolAllocator& other)
    {
        return m_pool.get() == other.m_pool.get();
    }

    bool operator!=(const PoolAllocator& other)
    {
        return m_pool.get() != other.m_pool.get();
    }
private:
    std::shared_ptr<Pool> m_pool;
    template <typename U, size_t>
    friend class PoolAllocator;
};