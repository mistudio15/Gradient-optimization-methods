#pragma once
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>

template <class U>
class Matrix
{
public:
    Matrix() = default;
    // ?? создание нулевой матрицы
    Matrix(size_t nRows_, size_t nCols_) : matrix(nRows_, std::vector<U>(nCols_, 0)), nRows(nRows_), nCols(nCols_) {};
    explicit Matrix(std::vector<std::vector<U>> const &matrix_) : matrix(matrix_), nRows(matrix.size()), nCols(matrix[0].size()) {};
    explicit Matrix(std::vector<std::vector<U>> &&matrix_) : matrix(std::move(matrix_)), nRows(matrix.size()), nCols(matrix[0].size()) {};
    // должно работать RVO или научиться std::move
    // Matrix(Matrix const &other) = delete; // 
    void Show();
    Matrix operator*(Matrix const &other) const;
    // Matrix * a, нужно a * Matrix
    Matrix operator*(float a) const;
    Matrix operator-(Matrix const &other) const;
    Matrix T() const;
    std::vector<U> &operator[](int index) const;
    size_t GetRows() const { return nRows; }
    size_t GetCols() const { return nCols; }
private:
    std::vector<std::vector<U>> matrix;
    size_t nRows;
    size_t nCols;
};

template <class U>
void Matrix<U>::Show()
{
    for (std::vector<U> &row : matrix)
    {
        for (U &elem : row)
        {
            std::cout << std::setw(5) <<  elem << " "; 
        }
        std::cout << std::endl;
    }
}

// ? метод const, возвращает ссылку, значение по которой можно изменить
template <class U>
std::vector<U> &Matrix<U>::operator[](int index) const
{
    return const_cast<std::vector<U> &>(matrix[index]);
}

// можно реализовать и модифицирующий алгоритм
template <class U>
Matrix<U> Matrix<U>::T() const
{
    Matrix<U> tMatrix(GetCols(), GetRows());
    for (size_t i = 0; i < GetRows(); i++)
    {
        for (size_t j = 0; j < GetCols(); j++)
        {
            tMatrix[j][i] = matrix[i][j];
        }
    }
    return tMatrix;
}

// на курсовой реализовать Штрассена
// происходит ли копирование объекта при возвращение или работает RVO
template <class U>
Matrix<U> Matrix<U>::operator*(Matrix<U> const &other) const 
{
    assert(this->GetCols() == other.GetRows());
    Matrix matrixProd(this->GetRows(), other.GetCols());
    for (size_t i = 0; i < this->GetRows(); ++i)
    {
        for (size_t j = 0; j < other.GetCols(); ++j)
        {
            U elem_i_j = 0; 
            for (size_t k = 0; k < this->GetCols(); ++k)
            {
                elem_i_j += matrix[i][k] * other.matrix[k][j];
            }
            matrixProd[i][j] = elem_i_j;
        }

    }
    return matrixProd;
}

template <class U>
Matrix<U> Matrix<U>::operator*(float a) const
{
    Matrix<U> aMatrix(GetCols(), GetRows());
    for (size_t i = 0; i < GetRows(); ++i)
    {
        for (size_t j = 0; j < GetCols(); ++j)
        {
            aMatrix[i][j] = a * matrix[i][j];
        }
    }
    return aMatrix;
} 

template <class U>
Matrix<U> Matrix<U>::operator-(Matrix const &other) const
{
    assert(this->GetRows() == other.GetRows() && this->GetCols() == other.GetCols());
    Matrix<U> diffMatrix(GetCols(), GetRows());
    for (size_t i = 0; i < GetRows(); ++i)
    {
        for (size_t j = 0; j < GetCols(); ++j)
        {
            diffMatrix[i][j] = matrix[i][j] - other.matrix[i][j];
        }
    }
    return diffMatrix;
}