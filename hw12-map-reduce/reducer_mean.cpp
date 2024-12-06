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
            std::cout << cnt << ", " << price << std::endl;
            sum += price;
            ++cnt;
        }
        catch(const std::invalid_argument& e)
        {}
    }
    double meanPrice = sum / cnt;
    std::cout << "Mean price = " << meanPrice << "$" << std::endl;
    std::cout << "Items processed = " << cnt << std::endl;
    return 0;
}