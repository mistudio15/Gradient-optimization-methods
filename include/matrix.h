#pragma once

#include "stdafx.h"

namespace Linalg
{
    template <class U>
    class Matrix;
    
    Matrix<double> ReadCSV(std::istream &file);
    
    template <class U>
    std::pair<Matrix<U>, Matrix<U>> SplitVertically(Matrix<U> const &original, size_t nPart1Cols);

    // попробовать написать для n разделений
    template <class U>
    std::pair<Matrix<U>, Matrix<U>> SplitHorizontally(Matrix<U>const &original, size_t nPart1Rows);
}


namespace Linalg
{

template <class U>
class Matrix
{
public:
    // constructor
    Matrix() = default;
    Matrix(size_t nRows_, size_t nCols_, U fill_value = 0) : matrix(nRows_, std::vector<U>(nCols_, fill_value)), nRows(nRows_), nCols(nCols_) {};
    // SetData проверяет матрицу на "прямоугольность"
    Matrix(typename std::vector<std::vector<U>>::const_iterator it_first, typename std::vector<std::vector<U>>::const_iterator it_last) { SetData(it_first, it_last); };
    explicit Matrix(std::vector<std::vector<U>> const &matrix_) { SetData(matrix_.begin(), matrix_.end()); };

    // не explicit для model.Predict({{{16, 79, 0, 1}}})
    Matrix(std::initializer_list<std::initializer_list<U>> const &matrix_) { SetData(matrix_.begin(), matrix_.end()); }

    // move   (+ может, стоит для vector или init_list сделать)
    Matrix(Matrix &&other) : matrix(std::move(other.matrix)), nRows(other.nRows), nCols(other.nCols) {};

    // copy
    Matrix(Matrix<U> const &other) : matrix(other.matrix), nRows(other.nRows), nCols(other.nCols) {};

    // =
    Matrix& operator=(Matrix<U> other);

    void Assign(size_t nRows_, size_t nCols_, U fill_value = 0);
    
    template <class Iter>
    void SetData(Iter const &it_first, Iter const &it_last);
    void SetData(std::vector<std::vector<U>> const &matrix_);
    void SetData(std::initializer_list<std::initializer_list<U>> const &matrix_);

    void AddRow(std::vector<U> const &row);
    void AddCol(U fill_value);
    void AddCol(std::vector<U> const &col);

    void       Show(std::string message = std::string{}) const;

    U          Sum()      const;
    Matrix     T()        const;
    double     Mean()     const;
    Matrix<U>  Abs()      const;
    Matrix<U>  Sqr()      const;
    size_t     GetRows()  const { return nRows; }
    size_t     GetCols()  const { return nCols; }

    std::vector<std::vector<U>> &Data() { return matrix; }
    typename std::vector<std::vector<U>>::const_iterator cIterRow(size_t indexRow) const
    { return matrix.begin() + indexRow;             }
    typename std::vector<std::vector<U>>::iterator IterRow(size_t indexRow)
    { return matrix.begin() + indexRow;             }
    typename std::vector<U>::const_iterator cIterCell(size_t indexRow, size_t indexCell) const
    { return matrix[indexRow].begin() + indexCell;  }
    typename std::vector<U>::iterator IterCell(size_t indexRow, size_t indexCell) 
    { return matrix[indexRow].begin() + indexCell;  }


    Matrix            operator*    (Matrix const &other) const;
    Matrix           &operator-=   (Matrix const &other);
    Matrix           &operator+=   (Matrix const &other);
    std::vector<U>   &operator[]   (int index);
    std::vector<U>    operator[]   (int index) const;
    template <class V>
    Matrix           &operator*=   (V a);
    template <class V>
    Matrix           &operator/=   (V a);
private:
    std::vector<std::vector<U>>    matrix;
    size_t                         nRows   = 0;
    size_t                         nCols   = 0;
    void swap(Matrix<U> &other);
};

template <class U>
Matrix<U> &Matrix<U>::operator+=(Matrix const &other)
{
    assert(this->GetRows() == other.GetRows() && this->GetCols() == other.GetCols());
    // сложение matrix и other.matrix и запись в matrix
    std::transform(matrix.begin(), matrix.end(), other.matrix.cbegin(), matrix.begin(), [](std::vector<U> &vecLeft, std::vector<U> const &vecRight){
        // сложение vecLeft и vecRight и запись в vecLeft
        std::transform(vecLeft.cbegin(), vecLeft.cend(), vecRight.cbegin(), vecLeft.begin(), std::plus<U>{});
        return vecLeft;
    });
    return *this;
}

template <class U>
Matrix<U> &Matrix<U>::operator-=(Matrix const &other)
{
    assert(this->GetRows() == other.GetRows() && this->GetCols() == other.GetCols());

    std::transform(matrix.begin(), matrix.end(), other.matrix.cbegin(), matrix.begin(), [](std::vector<U> &vecLeft, std::vector<U> const &vecRight){
        std::transform(vecLeft.cbegin(), vecLeft.cend(), vecRight.cbegin(), vecLeft.begin(), std::minus<U>{});
        return vecLeft;
    });
    return *this;
}


template <class U>
template <class V>
Matrix<U> &Matrix<U>::operator*=(V a)
{
    std::transform(matrix.begin(), matrix.end(), matrix.begin(), [a](std::vector<U> &row){
        std::for_each(row.begin(), row.end(), [a](U &elem){
            elem *= a;
        });
        return row;
    });
    return *this;
}

template <class U>
template <class V>
Matrix<U> &Matrix<U>::operator/=(V a)
{
    std::transform(matrix.begin(), matrix.end(), matrix.begin(), [a](std::vector<U> &row){
        std::for_each(row.begin(), row.end(), [a](U &elem){
            elem /= a;
        });
        return row;
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
std::vector<U> &Matrix<U>::operator[](int index)
{
    return matrix[index];
}

template <class U>
std::vector<U> Matrix<U>::operator[](int index) const
{
    return matrix[index];
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
            std::cout << std::setw(10) << std::setprecision(3) <<  elem << " "; 
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
    double sum = 0;
    for (auto const &row : matrix)
    {
        for (U const &elem : row)
        {
            sum += elem;
        }
    }
    return sum;
}

template <class U>
double Matrix<U>::Mean() const
{
    return static_cast<double>(Sum()) / (nRows * nCols);
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

template <class U>
Matrix<U> Matrix<U>::Sqr() const
{
    Matrix<U> sqrMatrix(*this);
    std::for_each(sqrMatrix.matrix.begin(), sqrMatrix.matrix.end(), [](std::vector<U> &vec){
        std::for_each(vec.begin(), vec.end(), [](U &elem){
            elem *= elem;
        });
    });
    return sqrMatrix;
}



// если делать move для init_list, то стоит эту функцию изменить
// matrix.push_back(std::move(*it));
template <class U>
template <class Iter>
void Matrix<U>::SetData(Iter const &it_first, Iter const &it_last)
{
    nRows = std::distance(it_first, it_last);
    nCols = (*it_first).size();
    matrix.clear();
    try
    {
        for (auto it = it_first; it != it_last; ++it)
        {
            if ((*it).size() != nCols)
            {
                throw (*it).size();
            }
            matrix.push_back(*it);
        }
    }
    catch(size_t nColsOther)
    {
        std::cout << "Exception\nWrong number of columns " << nColsOther << std::endl;
        exit(0);
    }
}

template <class U>
void Matrix<U>::SetData(std::vector<std::vector<U>> const &matrix_)
{
    SetData(matrix_.begin(), matrix_.end());
}

template <class U>
void Matrix<U>::SetData(std::initializer_list<std::initializer_list<U>> const &matrix_)
{
    SetData(matrix_.begin(), matrix_.end());
}

template <class U>
void Matrix<U>::AddRow(std::vector<U> const &row)
{
    assert(nRows == 0 || row.size() == GetCols());
    matrix.push_back(row);
    if (nRows == 0)
    {
        nCols = row.size();
    }
    nRows++;
}

template <class U>
void Matrix<U>::AddCol(U fill_value)
{
    for (auto &row : matrix)
    {
        row.push_back(fill_value);
    }
    nCols++;
}

template <class U>
void Matrix<U>::AddCol(std::vector<U> const &col)
{
    assert(col.size() == GetRows());
    for (size_t i = 0; i < col.size(); ++i)
    {
        matrix[i].push_back(col[i]);
    }    
    nCols++;
}

template <class U>
std::pair<Matrix<U>, Matrix<U>> SplitVertically(Matrix<U> const &original, size_t nPart1Cols)
{
    assert(nPart1Cols < original.GetCols());
    size_t nPart2Cols = original.GetCols() - nPart1Cols;
    Matrix<U> leftMatrix(original.GetRows(), nPart1Cols);
    Matrix<U> rightMatrix(original.GetRows(), nPart2Cols);
    for (size_t i = 0; i < original.GetRows(); ++i)
    {
        // copy до элемента, который следует после последнего
        std::copy(original.cIterCell(i, 0), original.cIterCell(i, nPart1Cols), leftMatrix[i].begin());
        std::copy(original.cIterCell(i, nPart1Cols), original.cIterCell(i, nPart1Cols + nPart2Cols), rightMatrix[i].begin());
    }
    return std::make_pair(leftMatrix, rightMatrix);
}

template <class U>
std::pair<Matrix<U>, Matrix<U>> SplitHorizontally(Matrix<U> const &original, size_t nPart1Rows)
{
    assert(nPart1Rows < original.GetRows());
    size_t nPart2Rows = original.GetRows() - nPart1Rows;
    Matrix<U> topMatrix(nPart1Rows, original.GetCols());
    Matrix<U> downMatrix(nPart2Rows, original.GetCols());
    std::copy(original.cIterRow(0), original.cIterRow(nPart1Rows), topMatrix.IterRow(0));
    std::copy(original.cIterRow(nPart1Rows), original.cIterRow(nPart1Rows + nPart2Rows), downMatrix.IterRow(0));
    return std::make_pair(topMatrix, downMatrix);
}

}