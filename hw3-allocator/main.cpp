#include <iostream>
#include <vector>

#include "linearallocator.h"

template<typename T>
struct SimpleAlloc
{
    using value_type = T;
    SimpleAlloc() = default;
    T* allocate(size_t n)
    {
        return reinterpret_cast<T*>(new char[n * sizeof(T)]);
    }
    void deallocate(T* p, size_t n)
    {
        delete [] reinterpret_cast<char*>(p);
    }
};

class Arena
{
public:
    /* @brief Allocate from pp[0, ss-1]. */
    Arena(char* pp, int ss) : p(pp), s(ss)
    {}
    ~Arena()
    {
        delete [] p;
    }
    char* p;
    int s;
};

template<typename T>
struct MyAlloc
{
    using value_type = T;
    MyAlloc(Arena &aa) : a(aa)
    {
        ptr = a.p;
    }
    MyAlloc() = delete;
    T* allocate(size_t n)
    {
        ptr+= n * sizeof(T);
        std::cout << ptr << std::endl;
        return reinterpret_cast<T*>(ptr);
    }
    void deallocate(T* p, size_t n)
    {
        ptr-= n * sizeof(T);
    }
    Arena &a;
    char* ptr;
};

template<typename T>
struct LoggingAlloctor
{
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    LoggingAlloctor() = default;
    ~LoggingAlloctor() = default;
    T* allocate(std::size_t n)
    {
        std::cout << "allocate: [n = " << n << "]" << std::endl;
        auto p = std::malloc(n * sizeof(T));
        if(!p)
        {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T*>(p);
    }
    void deallocate(T *p, std::size_t n)
    {
        std::cout << "deallocate: [n = " << n << "]" << std::endl;
        std::free(p);
    }
};

int main()
{
    try
    {
        // LinearAlloctor<int> linearAllocator(std::size_t(10));
        // std::vector<int, LinearAlloctor<int>> vec{std::move(linearAllocator)};
        std::vector<int, LinearAlloctor<int>> vec{std::move(LinearAlloctor<int>(std::size_t(10)))};
        std::cout << "Initial cap: " << vec.capacity() << std::endl;
        vec.push_back(11);
        std::cout << "Cap: " << vec.capacity() << std::endl;
        vec.push_back(22);
        std::cout << "Cap: " << vec.capacity() << std::endl;
        vec.push_back(33);
        std::cout << "Cap: " << vec.capacity() << std::endl;
        vec.push_back(44);
        std::cout << "Cap: " << vec.capacity() << std::endl;
        vec.pop_back();
        vec.push_back(55);
        std::cout << "Cap: " << vec.capacity() << std::endl;
        std::cout << "Vec values:" << std::endl;
        for(int value : vec)
        {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}