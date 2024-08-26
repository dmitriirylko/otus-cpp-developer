#pragma once

#include <stack>
#include <memory>
#include <span>

class pool
{
public:
    using size_type = std::size_t;
    explicit pool(size_type size) : m_size{size}
    {}
    void* allocate()
    {
        if(m_addrs.empty())
        {
            add_addrs();
        }
        auto ptr = m_addrs.top();
        m_addrs.pop();
        std::cout << ptr << std::endl;
        return ptr;
    }
    void deallocate(void *ptr)
    {
        m_addrs.push(ptr);
    }
    void rebind(size_type size)
    {
        if(!m_addrs.empty() || !m_blocks.empty())
        {
            std::cerr << "rebind after alloc is unsupported" << std::endl;
            abort();
        }
        m_size = size;
    }
    void add_addrs()
    {
        constexpr const auto block_size = 0x1000;
        auto block = std::make_unique<uint8_t[]>(block_size);
        auto v = std::span<uint8_t>(block.get(), block_size);
        auto total_size = v.size() % m_size == 0 ? v.size() : v.size() - m_size;
        for(auto i = 0; i < total_size; i += m_size)
        {
            m_addrs.push(&v[i]);
        }
        m_blocks.push(std::move(block));
    }

private:
    size_type m_size;
    std::stack<void*> m_addrs{};
    std::stack<std::unique_ptr<uint8_t[]>> m_blocks{};
};

template<typename T>
class poolallocator
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
    struct rebind {
        using other = poolallocator<U>;
    };
    poolallocator() : m_pool{std::make_shared<pool>(sizeof(T))}
    {
        std::cout << this << " constructor, sizeof(T): " << sizeof(T) << '\n';
    }
    template<typename U>
    poolallocator(const poolallocator<U>& other) noexcept : m_pool{other.m_pool}
    {
        std::cout << this << " copy constructor (U), sizeof(T): " << sizeof(T) << '\n';
        m_pool->rebind(sizeof(T));
    }
    poolallocator(poolallocator&& other) noexcept : m_pool{std::move(other.m_pool)}
    {
        std::cout << this << " move constructor, sizeof(T): " << sizeof(T) << '\n';
    }
    poolallocator& operator=(poolallocator&& other) noexcept
    {
        std::cout << this << " move assignment, sizeof(T): " << sizeof(T) << '\n';
        m_pool = std::move(other.m_pool);
        return *this;
    }
    poolallocator(const poolallocator& other) noexcept : m_pool{other.m_pool}
    {
        std::cout << this << " copy constructor, sizeof(T): " << sizeof(T) << '\n';
    }
    poolallocator& operator=(const poolallocator& other) noexcept
    {
        std::cout << this << " copy assignment, sizeof(T): " << sizeof(T) << '\n';
        m_pool = other.m_pool;
        return *this;
    }
    pointer allocate(size_type n)
    {
        std::cout << this << " A [" << n << "]: ";
        if(n != 1)
        {
            return static_cast<pointer>(malloc(sizeof(T) * n));
        }
        return static_cast<pointer>(m_pool->allocate());
    }
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
    std::shared_ptr<pool> m_pool;
    template<typename T1, typename T2>
    friend bool operator==(const poolallocator<T1> &lhs, const poolallocator<T2> &rhs);
    template<typename T1, typename T2>
    friend bool operator!=(const poolallocator<T1> &lhs, const poolallocator<T2> &rhs);
    template <typename U>
    friend class poolallocator;
};

template<typename T1, typename T2>
bool operator==(const poolallocator<T1> &lhs, const poolallocator<T2> &rhs)
{
    return lhs.m_pool.get() == rhs.m_pool.get();
}

template<typename T1, typename T2>
bool operator!=(const poolallocator<T1> &lhs, const poolallocator<T2> &rhs)
{
    return lhs.m_pool.get() != rhs.m_pool.get();
}
