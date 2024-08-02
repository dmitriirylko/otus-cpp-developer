#include <iostream>
#include <vector>

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
    // std::vector<int, SimpleAlloc<int>> vec;
    // vec.push_back(52);
    // vec.push_back(25);
    // std::cout << "SimpleAlloc values:" << std::endl;
    // std::cout << vec[0] << "\t" << vec[1] << std::endl;

    // constexpr int sz{100000};
    // Arena myArena1{new char[sz], sz};
    // Arena myArena2{new char[10 * sz], 10 * sz};
    // std::vector<int, MyAlloc<int>> vec1{MyAlloc<int>{myArena1}};
    // vec1.push_back(52);
    // vec1.push_back(522);
    // vec1.pop_back();
    // vec1.push_back(52);
    // vec1.push_back(13);
    // std::cout << "MyAlloc values:" << std::endl;
    // std::cout << vec1[0] << "\t" << vec1[1] << "\t" << vec1[2] << std::endl;
    // std::cout << myArena1.p << std::endl;
    // std::cout << &vec1[0] << "\t" << &vec1[1] << "\t" << &vec1[2] << std::endl;
    auto vec = std::vector<int, LoggingAlloctor<int>>{};
    for(int i = 0; i < 5; ++i)
    {
        std::cout << "vec size = " << vec.size() << std::endl;
        vec.emplace_back(i);
        std::cout << std::endl;
    }
    return 0;
}