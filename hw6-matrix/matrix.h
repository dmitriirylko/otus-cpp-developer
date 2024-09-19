#pragma once

#include <iostream>
#include <map>
#include <utility>

template<typename T, T defaultValue> class MatrixProxy;

template<typename T, T defaultValue>
class Matrix
{
public:
    Matrix() = default;

    ~Matrix() = default;

    size_t size()
    {
        return m_data.size();
    }

    void print()
    {
        for(auto const& x : m_data)
        {
            std::cout << x.first.first << ", " << x.first.second << ", " << x.second << std::endl;
        }
    }

    MatrixProxy<T, defaultValue> operator[](size_t row)
    {
        return MatrixProxy(*this, row);
    }

private:
    std::map<std::pair<size_t, size_t>, T> m_data;
    friend MatrixProxy<T, defaultValue>;
};

template<typename T, T defaultValue>
class MatrixProxy
{
public:
    MatrixProxy(Matrix<T, defaultValue>& matrix, size_t row) :
        m_matrix{matrix},
        m_row{row}
    {}

    ~MatrixProxy() = default;

    T& operator[](size_t col)
    {
        auto key = std::make_pair(m_row, col);
        if(m_matrix.m_data.find(key) == m_matrix.m_data.end())
        {
            return;
        }
        return m_matrix.m_data[key];
    } 

private:
    size_t m_row;
    Matrix<T, defaultValue>& m_matrix;
};