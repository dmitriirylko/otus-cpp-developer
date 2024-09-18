#include "matrix.h"

int main()
{
    Matrix<int, -1> matrix;
    std::cout << matrix.size() << std::endl;
    matrix[100][100] = 314;
    std::cout << matrix[100][100] << std::endl;
    std::cout << matrix.size() << std::endl;
    return 0;
}