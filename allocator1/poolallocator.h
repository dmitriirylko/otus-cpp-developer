#pragma once

#include <stack>
#include <queue>
#include <memory>

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
    {}
    /**
     * @brief Gets pointer out of the pool for further allocation. If there are no
     *          vacant addresses in pool it tries to allocate new memory block.
     * @return Pointer out of the pool for allocation.
     */
    void* allocate()
    {
        if(m_addrs.empty())
        {
            addAddrs();
        }
        auto ptr = m_addrs.front();
        m_addrs.pop();
        std::cout << ptr << std::endl;
        return ptr;
    }
    /**
     * @brief Returns deallocated pointer to addresses queue.
     */
    void deallocate(void *ptr)
    {
        m_addrs.push(ptr);
    }
    /**
     * @brief Changes size of data structure which is going to be allocated in pool.
     *          Rebinding after any allocation is unsupported.
     * @param Size of data structure which is going to be allocated in pool.
     */
    void rebind(std::size_t typeSize)
    {
        if(!m_addrs.empty() || !m_blocks.empty())
        {
            throw std::bad_alloc();
        }
        m_typeSize = typeSize;
    }
    void addAddrs()
    {
        size_t blockSize = m_elementsNum * m_typeSize;
        m_blocks.push(std::make_unique<uint8_t[]>(blockSize));
        uint8_t* ptr = m_blocks.top().get();
        for(size_t i = 0; i < blockSize; i += m_typeSize)
        {
            m_addrs.push(ptr);
            ptr += m_typeSize;
        }
        std::cout << " add_addrs ";
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
    std::queue<void*> m_addrs{};
    /**
     * @brief Stack of memory blocks. Block are added when previous
     *          one has been run out or there are no blocks in stack.
     */
    std::stack<std::unique_ptr<uint8_t[]>> m_blocks{};
};

template<typename T, size_t elementsNum = 10>
class PoolAllocator
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
        using other = PoolAllocator<U, elementsNum>;
    };
    PoolAllocator() : m_pool{std::make_shared<Pool>(sizeof(T), elementsNum)}
    {
        std::cout << this << " constructor, sizeof(T): " << sizeof(T) << '\n';
    }
    template<typename U>
    PoolAllocator(const PoolAllocator<U, elementsNum>& other) noexcept : m_pool{other.m_pool}
    {
        std::cout << this << " copy constructor (U), sizeof(T): " << sizeof(T) << '\n';
        m_pool->rebind(sizeof(T));
    }
    PoolAllocator(PoolAllocator&& other) noexcept : m_pool{std::move(other.m_pool)}
    {
        std::cout << this << " move constructor, sizeof(T): " << sizeof(T) << '\n';
    }
    PoolAllocator& operator=(PoolAllocator&& other) noexcept
    {
        std::cout << this << " move assignment, sizeof(T): " << sizeof(T) << '\n';
        m_pool = std::move(other.m_pool);
        return *this;
    }
    PoolAllocator(const PoolAllocator& other) noexcept : m_pool{other.m_pool}
    {
        std::cout << this << " copy constructor, sizeof(T): " << sizeof(T) << '\n';
    }
    PoolAllocator& operator=(const PoolAllocator& other) noexcept
    {
        std::cout << this << " copy assignment, sizeof(T): " << sizeof(T) << '\n';
        m_pool = other.m_pool;
        return *this;
    }
    pointer allocate(size_type n)
    {
        std::cout << this << " A [" << n << "]: ";
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
        std::cout << this << " D [" << n << "]: " << ptr << std::endl;
        if(n != 1)
        {
            free(ptr);
        }
        m_pool->deallocate(ptr);
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
    std::shared_ptr<Pool> m_pool;
    template<typename T1, typename T2>
    friend bool operator==(const PoolAllocator<T1> &lhs, const PoolAllocator<T2> &rhs);
    template<typename T1, typename T2>
    friend bool operator!=(const PoolAllocator<T1> &lhs, const PoolAllocator<T2> &rhs);
    template <typename U, size_t>
    friend class PoolAllocator;
};

template<typename T1, typename T2>
bool operator==(const PoolAllocator<T1> &lhs, const PoolAllocator<T2> &rhs)
{
    return lhs.m_pool.get() == rhs.m_pool.get();
}

template<typename T1, typename T2>
bool operator!=(const PoolAllocator<T1> &lhs, const PoolAllocator<T2> &rhs)
{
    return lhs.m_pool.get() != rhs.m_pool.get();
}