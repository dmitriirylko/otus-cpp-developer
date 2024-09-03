/**
 * @file printip.h
 * @brief Template function that prints IP address-like data structures.
 * @author dmitriirylko
 */
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>

template<typename T,
         typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
void print_ip(T t)
{
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
         typename std::enable_if_t<std::is_same_v<T, std::string>, int> = 0>
void print_ip(const T& t)
{
    std::cout << t << std::endl;
}

template<
        template<typename, typename> typename Container,
        typename T,
        typename Allocator = std::allocator<T>,
        typename std::enable_if_t<std::is_same_v<Container<T, Allocator>, std::vector<T, Allocator>> ||
                                  std::is_same_v<Container<T, Allocator>, std::list<T, Allocator>>, int> = 0>
void print_ip(const Container<T, Allocator>& t)
{
    size_t counter = 0;
    for(const auto& v : t)
    {
        if(counter) std::cout << ".";
        std::cout << v;
        ++counter;
    }
    std::cout << std::endl;
}

template<size_t I,
         typename... Ts,
         typename std::enable_if_t<I == sizeof...(Ts), int> = 0>
void print_ip(std::tuple<Ts...> t)
{
    std::cout << std::endl;
    return;
}

template<size_t I = 0,
         typename... Ts,
         typename std::enable_if_t<I < sizeof...(Ts), int> = 0>
void print_ip(std::tuple<Ts...> t)
{
    using Tuple = std::tuple<Ts...>;
    static_assert(std::is_same_v<std::tuple_element_t<0, Tuple>, std::tuple_element_t<I, Tuple>>, "Type of tuple elements are not the same");
    if(I) std::cout << ".";
    std::cout << std::get<I>(t);
    print_ip<I + 1>(t);
}