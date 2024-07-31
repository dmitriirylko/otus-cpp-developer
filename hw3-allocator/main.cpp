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

int main()
{
    std::vector<int, SimpleAlloc<int>> vec;
    vec.push_back(52);
    vec.push_back(25);
    std::cout << "allocators" << std::endl;
    std::cout << vec[0] << "\t" << vec[1] << std::endl;
    return 0;
}