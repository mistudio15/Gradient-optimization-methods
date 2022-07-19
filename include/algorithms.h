// #pragma once

// #include "matrix.h"

// class GradientDescent
// {
// public:
//     GradientDescent(float lr_ = 0.1) : lr(lr_) {};
//     template <class U>
//     void Fit(Matrix<U> const &X_train, Matrix<U> const &y_train) 
//     {
//         w(X_train.GetCols(), 0);
//         for (size_t i = 0; i < 10; ++i)
//         {
//             Matrix<float> f = X_train * w;
//             Matrix<float> grad = X_train.T() * (y_train - f) * 2;
//             w = w - grad * lr; 
//         }
//         /*
//         a * Matrix
//         -=
//         Реализовать много операторов
//         Matrix заменить на универсальное название, представляющее вектор и матрицу

//         vector<float> grad = 2 * X.T() * (y - f);
//         w -= lr * grad; 
//         */
//     }
//     Matrix<float> Predict(Matrix<float> const &X_test) const
//     {
//         return X_test * w;
//     }

// private:
//     float lr;
//     // вектор транспонирован (для оптимизации хранения)
//     Matrix<float> w;
// };