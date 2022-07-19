#pragma once
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <algorithm>
#include <iterator>

template <class U>
class Matrix
{
public:
    Matrix() = default;
    Matrix(size_t nRows_, size_t nCols_) : matrix(nRows_, std::vector<U>(nCols_, 0)), nRows(nRows_), nCols(nCols_) {};
    explicit Matrix(std::vector<std::vector<U>> const &matrix_) : matrix(matrix_), nRows(matrix.size()), nCols(matrix[0].size()) {};
    Matrix(std::vector<std::vector<U>> &&matrix_) : matrix(std::move(matrix_)), nRows(matrix.size()), nCols(matrix[0].size()) {};
    Matrix(Matrix &&other);
    Matrix(Matrix<U> const &other);
    Matrix& operator=(Matrix<U> other);

    void Show();


    Matrix operator*(Matrix const &other) const;
    Matrix operator-(Matrix const &other) const;
    Matrix T() const;
    std::vector<U> &operator[](int index) const;
    size_t GetRows() const { return nRows; }
    size_t GetCols() const { return nCols; }
    ~Matrix() { std::cout << "destr()" << std::endl; }

    template <class T>
    Matrix operator*(T a) const
    {
        Matrix<U> aMatrix(GetRows(), GetCols());
        std::transform(matrix.begin(), matrix.end(), aMatrix.matrix.begin(), [a](std::vector<U> const &row){
            return row * a;
        });
        return aMatrix;
    }

    template <class T>
    friend Matrix operator*(T a, Matrix<U> const &original)
    {
        return original * a;
    }
private:
    std::vector<std::vector<U>> matrix;
    size_t nRows;
    size_t nCols;
    void swap(Matrix<U> &other);
};

template <class U, class T>
std::vector<U> operator*(std::vector<U> vec, T a)
{
    std::for_each(vec.begin(), vec.end(), [a](U &elem){
        elem = elem * a;
    });
    return vec;
}

template <class U>
Matrix<U>::Matrix(Matrix &&other)
{
    matrix = std::move(other.matrix);
    nRows = nRows;
    nCols = nCols;
}

template <class U>
void Matrix<U>::swap(Matrix<U> &other)
{
    std::swap(matrix, other.matrix);
    std::swap(nRows, other.nRows);
    std::swap(nCols, other.nCols);
}

template <class U>
Matrix<U>::Matrix(Matrix<U> const &other)
{
    matrix = other.matrix;
    nRows = other.nRows;
    nCols = other.nCols;
}

template <class U>
Matrix<U>& Matrix<U>::operator=(Matrix<U> other)
{
    swap(other);
    return *this;
}

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

template <class U>
std::vector<U> &Matrix<U>::operator[](int index) const
{
    return const_cast<std::vector<U> &>(matrix[index]);
}

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