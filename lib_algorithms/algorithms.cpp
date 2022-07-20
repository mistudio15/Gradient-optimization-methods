#include <functional>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "algorithms.h"
#include "matrix.h"


void GradientDescent::Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) 
{
    // y_train должен быть транспонирован
    assert(X_train.GetRows() == y_train.GetRows());
    w.Assign(X_train.GetCols(), 1, 0);
    Linalg::Matrix<double> f;
    Linalg::Matrix<double> err;
    Linalg::Matrix<double> grad;
    int N = 10;
    for (size_t i = 0; i < N; ++i)
    {
        f = X_train * w;
        // f.Show("\t\tX_train * w");

        err = f - y_train;
        // err.Show("\t\terr");
        std::cout << err.Sum() << std::endl;

        grad = (2 * (X_train.T() * err)) / X_train.GetRows();
        // grad.Show("\t\tgrad");

        w -= lr * grad;
        // w.Show("\t\t-----------------------w");
    }

}

Linalg::Matrix<double> GradientDescent::Predict(Linalg::Matrix<double> const &X_test) const
{
    assert(X_test.GetCols() == w.GetRows());
    w.Show();
    return X_test * w;
}

template <class T>
int sign(T value)
{
    return value > 0 ? 1 : -1; 
}

double FindExtremum(std::function<double(double)> const &df, std::function<double(double const &, double const &)> const &BinOp, double eps)
{
    srand(time(nullptr));
    double x_prev;
    // x - ключевой параметр!, если в f(x) крутизна очень большая, то возможно x прыгнет слишком далеко в сторону антиградиента, в точку, где крутизна еще больше, и так далее пока x не станет +- бесконечность... 
    double x = 15;
    // количество итераций сближения
    int N = 1000;
    double lr = 0.004;
    double c = 10000;
    double m = lr * 200000; // чем больше, тем дольше сходится
    int n{0};
    // do
    // {
    //     ++n;
    //     // lr = 1 / std::min(double(n + 1), c);
    //     lr = lr * std::exp(- n / m);
    //     x_prev = x;
    //     std::cout << "x = " << x << "\n\t\t\t\t\t\tlr = " << lr << std::endl;

    //     // x = BinOp(x, lr * sign(df(x)));
    //     x = BinOp(x, lr * df(x));
    //     std::cout << "x = " << x << " x_prev = " << x_prev << std::endl;
    //     // if (n > 10) break;
    // } while (fabs(x_prev - x) > eps);
    for (size_t i = 0; i < N; ++i)
    {
       ++n;
        lr = 1 / std::min(double(n + 1000), c);
        // lr = lr * std::exp(- n / m);
        x_prev = x;

        std::cout << "x = " << x << "\n\t\t\t\t\t\tlr = " << lr << " f = " << lr * df(x) << std::endl;

        // sign(df(x)) не учитывает крутизну, поэтому если попали в очень крутой участок, а до минимума далеко, идти будем долго...
        // x = BinOp(x, lr * sign(df(x)));
        x = BinOp(x, lr * df(x));
        std::cout << "x = " << x << " x_prev = " << x_prev << std::endl;
        // if (n > 10) break; 
    }
    std::cout << "n = " << n << std::endl;
    return x;
}