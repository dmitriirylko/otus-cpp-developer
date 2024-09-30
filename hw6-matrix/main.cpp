#include "matrix.h"

int main()
{
    Matrix<int, -1> matrix;
    std::cout << matrix.size() << std::endl;
    matrix[100][100] = 314;
    std::cout << matrix[0][0] << std::endl;
    std::cout << matrix.size() << std::endl;
    // matrix[100][100] = -1;
    // std::cout << matrix[100][100] << std::endl;
    std::cout << matrix.size() << std::endl;
    for(auto c: matrix)
    {
        std::cout << c.first.first << ", " << c.first.second << ", " << c.second << std::endl;
    }
    return 0;
}