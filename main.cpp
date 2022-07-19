#include <iostream>

#include "matrix.h"
#include "algorithms.h"

int main()
{
    Linalg::Matrix<float> m = {
        {
            {1, 2},
            {4, 5},
            {7, 8}
        }
    };

    Linalg::Matrix<float> m2 = {
        {
            {56, 2345},
            {44, 455},
            {4357, 458}
        }
    };

    m2 = 10 * m2;
    m2.Show();
    Linalg::Matrix<float> h = {
        {
            {3, 2, 5},
            {2, 1, 2}
        }
    };
    m2 = m2 - m;
    m2.Show();

}