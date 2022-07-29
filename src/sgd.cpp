#include "algorithms.h"
#include "matrix.h"

void SGD::Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) 
{
    assert(X_train.GetRows() == y_train.GetRows());    
    // w = WeightsMinErr(X_train, y_train); // w = (0,...,0) ?
    w.Assign(X_train.GetCols(), 1); // fill 0

    double originalErr = MAE(X_train * w, y_train);
    double Q = MSE(X_train * w, y_train);
    // std::cout << "Q : " << Q << std::endl;
    double prevQ = Q;

    Linalg::Matrix<double> f;
    Linalg::Matrix<double> err;
    Linalg::Matrix<double> grad;
    int N = 10000;
    int B = 40;
    double fr = 0.005;

    auto itX = X_train.cIterRow(0);
    auto itY = y_train.cIterRow(0);
    Linalg::Matrix<double> X_batch;
    Linalg::Matrix<double> y_batch;
    size_t i = 0;
    auto start = std::chrono::steady_clock::now();
    for (; i < N; ++i)
    {
        // - реализовать случайный выбор B объектов из выбоки
        for (size_t j = B; j < X_train.GetRows(); j += B)
        {
            X_batch.SetData(itX + (j - B), itX + j);
            y_batch.SetData(itY + (j - B), itY + j);
            f = X_batch * w;
            err = std::move(f) - y_batch;
            // "делить на X_batch.GetRows()" - это действие принесло 2.14 на numeric_scaled (было 2.36)
            grad = 2 * (X_batch.T() * err) / X_batch.GetRows();
            w -= lr * std::move(grad);
            Q = fr * MSE(err) + (1 - fr) * Q;
        }
        if (std::abs(prevQ - Q) < 100000000) 
        {
            break;
        }
        prevQ = Q;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << std::setw(20) << "Elapsed time = " << elapsed_seconds.count() << std::endl;
    std::cout << std::setw(20) << "MAE = " << MAE(X_train * w, y_train) << std::endl;
    std::cout << std::setw(20) << "original MAE = " << originalErr << std::endl; 
    std::cout << std::setw(20) << "Itarations = " << i << std::endl;
}

Linalg::Matrix<double> SGD::Predict(Linalg::Matrix<double> const &X_test) const
{
    assert(X_test.GetCols() == w.GetRows());
    return X_test * w;
}