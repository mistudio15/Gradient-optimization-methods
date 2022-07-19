#include <iostream>

#include "matrix.h"
#include "algorithms.h"

int main()
{
    Matrix<float> m = {
        {
            {1, 2},
            {4, 5},
            {7, 8}
        }
    };
    m.Show();
    m = 4 * m;
    m.Show();
    // Matrix<int> h = {
    //     {
    //         {3, 2, 5},
    //         {2, 1, 2}
    //     }
    // };

    // Matrix<int> hT = h.T();
    // puts("________________");
    // hT.Show();
    // // h.T().Show();
    // Matrix<int> mh = m * h;
    // mh.Show();
    
}