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
    double originalErr = AE(X_train * w, y_train);
    // double end = 10000;
    // double start = 1000;
    // double clr = 0.0001;
    // double rateLr = 10000;
    Linalg::Matrix<double> f;
    Linalg::Matrix<double> err;
    Linalg::Matrix<double> grad;
    double prevErr = 0;
    int N = 1000000;
    size_t i = 0;
    for (; i < N; ++i)
    {
        // попытки варьировать lr
        // lr = std::max(clr * std::exp(((double)i + 1) / rateLr), 0.00001);
        // lr = clr * std::log10(10 + 0.0001 * i);
        // lr = 1 / std::min(double(i + start), end);
        // std::cout << "\t\t\t\tlr = " << lr << std::endl;

        f = X_train * w;
        err = f - y_train;
        grad = (2 * (X_train.T() * err)) / X_train.GetRows();
        w -= lr * grad;

        
        std::cout << "err = " << err.Abs().Sum() << std::endl;
        // если текущая ошибка в 100 раз больше чем исходная, завершаем
        if (err.Abs().Sum() > originalErr * 100)
        {
            
            break;
        }
        // если 100 итераций назад была такая же ошибка, завершаем
        if (i % 100 == 0)
        {
            double curErr = err.Abs().Sum();
            if (prevErr == curErr)
            {
                break;
            }
            prevErr = curErr;
        }
        // sleep(1);
    }
    w.Show("\t\tweights");
    std::cout << "AE = " << err.Abs().Sum() << std::endl;
    std::cout << "original AE = " << originalErr << std::endl; 
    std::cout << "Itarations = " << i << std::endl;
}

Linalg::Matrix<double> GradientDescent::Predict(Linalg::Matrix<double> const &X_test) const
{
    assert(X_test.GetCols() == w.GetRows());
    w.Show();
    return X_test * w;
}
