#include "algorithms.h"
#include "matrix.h"

void SGD::Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) 
{
    lr = 0.00000451;
    // лучше перемешивание делать в вызывающей прогрмме, потому что иначе нужно создавать копию
    std::mt19937 rdnX(123);
    std::mt19937 rdnY(123);
    Linalg::Matrix<double> X_copy = X_train;
    Linalg::Matrix<double> y_copy = y_train;
    std::shuffle(X_copy.Data().begin(), X_copy.Data().end(), rdnX);
    std::shuffle(y_copy.Data().begin(), y_copy.Data().end(), rdnY);
    
    w = WeightsMinErr(X_copy, y_copy);
    
    Linalg::Matrix<double> f;
    Linalg::Matrix<double> err;
    Linalg::Matrix<double> prevErr;
    Linalg::Matrix<double> grad;
    int N = 10000;
    int B = 20;
    for (size_t i = 0; i < N; ++i)
    {
        auto itX = X_copy.Data().cbegin();
        auto itY = y_copy.Data().cbegin();
        for (size_t j = B; j < X_copy.GetRows(); j += B)
        {
            Linalg::Matrix<double> X_batch;
            Linalg::Matrix<double> y_batch;
            // нужна инициализация через итераторы
            X_batch.Copy(itX + (j - B), itX + j);
            y_batch.Copy(itY + (j - B), itY + j);
            f = X_batch * w;
            err = f - y_batch;
            grad = (2 * (X_batch.T() * err));
            w -= lr * grad;

            std::cout << "err = " << err.Abs().Sum() << std::endl;
        }
    }
    std::cout << SumErr(X_copy, y_copy, w) << std::endl;
}

Linalg::Matrix<double> SGD::Predict(Linalg::Matrix<double> const &X_test) const
{
    assert(X_test.GetCols() == w.GetRows());
    return X_test * w;
}