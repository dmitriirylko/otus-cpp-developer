#include <cstring>
#include <stdlib.h>
#include <new>

template< typename T>
class myallocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    using is_always_equal = std::true_type;
    
    myallocator()
    {
        std::cout << this << " constructor, sizeof(T): " << sizeof(T) << std::endl;
    }
    template<typename U>
    myallocator(const myallocator<U> &other) noexcept
    {
        std::cout << this << " copy ctor, sizeof(T): " << sizeof(T) << std::endl;
        (void)other;
    }
    myallocator& operator=(const myallocator &other) noexcept
    {
        std::cout << this << " copy assignment, sizeof(T): " << sizeof(T) << std::endl;
        (void)other;
    }
    myallocator(myallocator &&other) noexcept
    {
        std::cout << this << " move ctor, sizeof(T): " << sizeof(T) << std::endl;
        (void)other;
    }
    myallocator& operator=(myallocator &&other) noexcept
    {
        std::cout << this << " move assignment, sizeof(T): " << sizeof(T) << std::endl;
        (void)other;
    }
    pointer allocate(size_type n)
    {
        auto ptr = static_cast<pointer>(malloc(sizeof(T) * n));
        if(!ptr)
        {
            throw std::bad_alloc();
        }
        std::cout << this << " A [" << n << "]: " << ptr << std::endl;
        return ptr;
    }
    void deallocate(pointer p, size_type n)
    {
        (void)n;
        std::cout << this << " D [" << n << "]: " << p << std::endl;
        free(p);
    }
private:

};

template<typename T, typename U>
bool operator==(const myallocator<T>&, const myallocator<U>&)
{
    return true;
}

template<typename T, typename U>
bool operator!=(const myallocator<T>&, const myallocator<U>&)
{
    return false;
}