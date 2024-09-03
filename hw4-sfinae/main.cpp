#include <iostream>
#include <string>

template<typename T,
         typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
void print_ip(T t)
{
    std::cout << "Integral overload called: ";
    auto pt = reinterpret_cast<uint8_t*>(&t) + sizeof(T) - 1;
    for(size_t i = 0; i < sizeof(T); ++i)
    {
        if(i) std::cout << ".";
        std::cout << +(*pt);
        --pt;
    }
    std::cout << std::endl;
}

template<typename T,
         typename std::enable_if<std::is_same<T, std::string>::value, int>::type = 0>
void print_ip(const T& t)
{
    std::cout << "String overload called: ";
    std::cout << t << std::endl;
}

int main()
{
    print_ip(int8_t{-1});
    print_ip(int16_t{0});
    print_ip(int32_t{2130706433});
    print_ip(int64_t{8875824491850138409});
    print_ip(std::string{"Hello"});
    return 0;
}