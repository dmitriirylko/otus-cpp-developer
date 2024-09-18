#pragma once

#include <iostream>
#include <map>
#include <utility>

template<typename T, T defaultValue>
class Matrix
{
private:
    class MatrixProxy
    {
    public:
        MatrixProxy(Matrix* matrix, size_t row) :
            m_ptr(matrix),
            m_row(row)
        {}

        T& operator[](size_t col)
        {
            return m_ptr->m_data[std::make_pair(m_row, col)];
        } 

    private:
        size_t m_row;
        Matrix* m_ptr;
    };

public:
    Matrix() = default;

    ~Matrix() = default;
    size_t size()
    {
        return m_data.size();
    }

    void add(size_t row, size_t col, T val)
    {
        m_data[std::make_pair(row, col)] = val;
    }

    void print()
    {
        for(auto const& x : m_data)
        {
            std::cout << x.first.first << ", " << x.first.second << ", " << x.second << std::endl;
        }
    }

    MatrixProxy operator[](size_t row)
    {
        return MatrixProxy(this, row);
    }

private:
    std::map<std::pair<size_t, size_t>, T> m_data;
};