#include <iostream>
#include <string>

int main(int argc, char ** argv)
{
    std::string line;
    size_t cnt = 0;
    // std::cout << "makefile test" << std::endl;
    while(std::getline(std::cin, line))
    {
        ++cnt;
        std::cout << "Number of lines is " << cnt << std::endl;
    }

    return 0;
}