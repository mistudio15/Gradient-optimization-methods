#include "algorithms.h"
#include "matrix.h"


template <class T>
int sign(T value)
{
    return value > 0 ? 1 : -1; 
}

void GradientDescent::Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) 
{
    assert(X_train.GetRows() == y_train.GetRows());
    assert(X_train.GetRows() == y_train.GetRows());
    w = WeightsMinErr(X_train, y_train);
    // w.Assign(X_train.GetCols(), 1); 
    double originalQ       = MAE(X_train * w, y_train);
    double Q               = originalQ;
    double prevQ           = originalQ;
    Linalg::Matrix<double>   f;
    Linalg::Matrix<double>   err;
    Linalg::Matrix<double>   grad;
    double prevErr = 0;
    int N = 1000000;
    auto start = std::chrono::steady_clock::now();
    size_t i = 0;
    for (; i < N; ++i)
    {
        // попытки варьировать lr
        // lr = std::max(clr * std::exp(((double)i + 1) / rateLr), 0.00001);
        // lr = clr * std::log10(10 + 0.0001 * i);
        // lr = 1 / std::min(double(i + start), end);

        f = X_train * w;
        err = std::move(f) - y_train;
        grad = (2 * (X_train.T() * err)) / X_train.GetRows();
        w -= lr * std::move(grad);
        
        // std::cout << "Q = " << MAE(err) << std::endl;
        Q = MAE(X_train * w, y_train);
        if (Q > originalQ * 100)
        {
            break;
        }
        if (std::abs(prevQ - Q) < 100) 
        {
            break;
        }
        prevQ = Q;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << std::setw(20) << "Elapsed time = " << elapsed_seconds.count() << std::endl;
    std::cout << std::setw(20) << "MAE = " << MAE(err) << std::endl;
    std::cout << std::setw(20) << "original MAE = " << originalQ << std::endl; 
    std::cout << std::setw(20) << "Itarations = " << i << std::endl;
}

Linalg::Matrix<double> GradientDescent::Predict(Linalg::Matrix<double> const &X_test) const
{
    assert(X_test.GetCols() == w.GetRows());
    return X_test * w;
}
