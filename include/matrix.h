#pragma once
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <iterator>

namespace Linalg
{

template <class U>
class Matrix
{
public:
    // constructor
    Matrix() = default;
    Matrix(size_t nRows_, size_t nCols_) : matrix(nRows_, std::vector<U>(nCols_, 0)), nRows(nRows_), nCols(nCols_) {};
    explicit Matrix(std::vector<std::vector<U>> const &matrix_) : matrix(matrix_), nRows(matrix.size()), nCols(matrix[0].size()) {};
    // move
    Matrix(std::vector<std::vector<U>> &&matrix_) : matrix(std::move(matrix_)), nRows(matrix.size()), nCols(matrix[0].size()) {};
    Matrix(Matrix &&other) : matrix(std::move(other.matrix)), nRows(other.nRows), nCols(other.nCols) {};
    // copy
    Matrix(Matrix<U> const &other) : matrix(other.matrix), nRows(other.nRows), nCols(other.nCols) {};
    Matrix& operator=(Matrix<U> other);
    // инициализация через лист инициализации (вектор)
    // Matrix<U>& Matrix<U>::operator=(std::vector<U> vec);

    // реализовать fill со значениями генерируемыми функтором 
    void Assign(size_t nRows_, size_t nCols_, U fill_value = 0);

    void Show(std::string message = std::string{}) const;
    Matrix T() const;
    U Sum() const;
    Matrix<U> Abs() const;
    size_t GetRows() const { return nRows; }
    size_t GetCols() const { return nCols; }
    // ~Matrix() {}
    
    Matrix operator*(Matrix const &other) const;
    Matrix &operator-=(Matrix const &other);
    Matrix &operator+=(Matrix const &other);
    std::vector<U> &operator[](int index) const;
    template <class V>
    Matrix &operator*=(V a);
    template <class V>
    Matrix &operator/=(V a);
private:
    std::vector<std::vector<U>> matrix;
    size_t nRows;
    size_t nCols;
    void swap(Matrix<U> &other);
};


// vector functions

template <class U, class T>
std::vector<U> &operator*=(std::vector<U> &vec, T a)
{
    std::for_each(vec.begin(), vec.end(), [a](U &elem){
        elem *= a;
    });
    return vec;
}

template <class U, class T>
std::vector<U> &operator/=(std::vector<U> &vec, T a)
{
    std::for_each(vec.begin(), vec.end(), [a](U &elem){
        elem /= a;
    });
    return vec;
}

template <class U>
std::vector<U> &operator-=(std::vector<U> &vecLeft, std::vector<U> const &vecRight)
{
    std::transform(vecLeft.cbegin(), vecLeft.cend(), vecRight.cbegin(), vecLeft.begin(), std::minus<U>{});
    return vecLeft;
}

template <class U>
std::vector<U> &operator+=(std::vector<U> &vecLeft, std::vector<U> const &vecRight)
{
    std::transform(vecLeft.cbegin(), vecLeft.cend(), vecRight.cbegin(), vecLeft.begin(), std::plus<U>{});
    return vecLeft;
}

// end vector functions

template <class U>
template <class V>
Matrix<U> &Matrix<U>::operator*=(V a)
{
    std::transform(matrix.begin(), matrix.end(), matrix.begin(), [a](std::vector<U> &row){
        return (row *= a);
    });
    return *this;
}

template <class U>
template <class V>
Matrix<U> &Matrix<U>::operator/=(V a)
{
    std::transform(matrix.begin(), matrix.end(), matrix.begin(), [a](std::vector<U> &row){
        return (row /= a);
    });
    return *this;
}

template <class U>
Matrix<U> &Matrix<U>::operator-=(Matrix const &other)
{
    assert(this->GetRows() == other.GetRows() && this->GetCols() == other.GetCols());
    std::transform(matrix.begin(), matrix.end(), other.matrix.cbegin(), matrix.begin(), [](std::vector<U> &left, std::vector<U> const &right){
        return (left -= right);
    });
    return *this;
}

template <class U>
Matrix<U> &Matrix<U>::operator+=(Matrix const &other)
{
    assert(this->GetRows() == other.GetRows() && this->GetCols() == other.GetCols());
    std::transform(matrix.begin(), matrix.end(), other.matrix.cbegin(), matrix.begin(), [](std::vector<U> &left, std::vector<U> const &right){
        return (left += right);
    });
    return *this;
}

// no-member

template <class U>
Matrix<U> operator-(Matrix<U> left, Matrix<U> const &right)
{
    assert(left.GetRows() == right.GetRows() && left.GetCols() == right.GetCols());
    return left -= right;
}

template <class U>
Matrix<U> operator+(Matrix<U> left, Matrix<U> const &right)
{
    assert(left.GetRows() == right.GetRows() && left.GetCols() == right.GetCols());
    return left += right;
}

template <class U, class V>
Matrix<U> operator*(Matrix<U> left, V a)
{
    return left *= a;
}

template <class U, class V>
Matrix<U> operator*(V a, Matrix<U> const &right)
{
    return right * a;
}


template <class U, class V>
Matrix<U> operator/(Matrix<U> left, V a)
{
    return left /= a;
}

// member

template <class U>
std::vector<U> &Matrix<U>::operator[](int index) const
{
    return const_cast<std::vector<U> &>(matrix[index]);
}

// на курсовой реализовать Штрассена при большой матрице
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
Matrix<U>& Matrix<U>::operator=(Matrix<U> other)
{
    swap(other);
    return *this;
}

// template <class U>
// Matrix<U>& Matrix<U>::operator=(std::vector<U> vec)
// {
//     swap(other);
//     return *this;
// }

template <class U>
void Matrix<U>::Assign(size_t nRows_, size_t nCols_, U fill_value)
{
    nRows = nRows_;
    nCols = nCols_;
    matrix.assign(nRows, std::vector<U>(nCols, fill_value));
}

template <class U>
void Matrix<U>::swap(Matrix<U> &other)
{
    std::swap(matrix, other.matrix);
    std::swap(nRows, other.nRows);
    std::swap(nCols, other.nCols);
}

// сделать через copy std::ostream_iterator
template <class U>
void Matrix<U>::Show(std::string message) const
{
    std::cout << message << std::endl;
    for (std::vector<U> const &row : matrix)
    {
        for (U const &elem : row)
        {
            std::cout << std::setw(5) <<  elem << " "; 
        }
        std::cout << std::endl;
    }
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

template <class U>
U Matrix<U>::Sum() const
{
    return std::accumulate(matrix.begin(), matrix.end(), 0, [](U init, std::vector<U> const &vec){
        U sum = std::accumulate(vec.begin(), vec.end(), 0);
        return init + sum;
    });
}

template <class U>
Matrix<U> Matrix<U>::Abs() const
{
    Matrix<U> absMatrix(*this);
    std::for_each(absMatrix.matrix.begin(), absMatrix.matrix.end(), [](std::vector<U> &vec){
        std::for_each(vec.begin(), vec.end(), [](U &elem){
            elem = (elem > 0 ? elem : -elem);
        });
    });
    return absMatrix;
}

}