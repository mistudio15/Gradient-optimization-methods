#include <iostream>

#include "matrix.h"
#include "algorithms.h"

int main()
{
    Matrix<int> m({
        {1, 2},
        {4, 5},
        {7, 8}
    });
    m.Show();
    puts("");
    Matrix<int> h({
        {3, 2, 5},
        {2, 1, 2}
    });
    puts("");
    h.Show();
    puts("");
    h.T().Show();
    Matrix<int> mh = m * h;
    mh.Show();
}