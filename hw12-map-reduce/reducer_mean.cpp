#include <iostream>
#include <string>

int main(int argc, char ** argv)
{
    size_t cnt = 0;
    double sum = 0;
    std::string line;
    double price;
    while(std::getline(std::cin, line))
    {
        try
        {
            price = std::stod(line);
            sum += price;
            ++cnt;
        }
        catch(const std::invalid_argument& e)
        {}
    }
    double meanPrice = sum / cnt;
    std::cout << meanPrice;
    return 0;
}