#include "algorithms.h"
#include "matrix.h"

// [no-mean] Absolute error
double AE(Linalg::Matrix<double> const &y, Linalg::Matrix<double> const &y_pred)
{
    assert(y.GetCols() == 1 && y_pred.GetCols() == 1 && y.GetRows() == y_pred.GetRows());
    return (y_pred - y).Abs().Sum()[0][0];
}

double MSE(Linalg::Matrix<double> const &y, Linalg::Matrix<double> const &y_pred)
{
    assert(y.GetCols() == 1 && y_pred.GetCols() == 1 && y.GetRows() == y_pred.GetRows());
    return (y_pred - y).Sqr().Mean()[0][0];
}

double MAE(Linalg::Matrix<double> const &y, Linalg::Matrix<double> const &y_pred)
{
    assert(y.GetCols() == 1 && y_pred.GetCols() == 1 && y.GetRows() == y_pred.GetRows());
    return (y_pred - y).Abs().Mean()[0][0];
}

double AE(Linalg::Matrix<double> const &err)
{
    assert(err.GetCols() == 1);
    return err.Abs().Sum()[0][0];
}

double MSE(Linalg::Matrix<double> const &err)
{
    assert(err.GetCols() == 1);
    return err.Sqr().Mean()[0][0];
}

double MAE(Linalg::Matrix<double> const &err)
{
    assert(err.GetCols() == 1);
    return err.Abs().Mean()[0][0];
}


Linalg::Matrix<double> WeightsMinErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train)
{
    std::random_device rd;
    std::mt19937 rnd(rd());
    std::uniform_real_distribution<> dist(-10000, 30000);
    Linalg::Matrix<double> minW(X_train.GetCols(), 1, 0); // fill 0
    double minSumErr = AE(X_train * minW, y_train);
    double curSumErr = 0;
    for (size_t i = 0; i < 100; ++i)
    {
        Linalg::Matrix<double> curW(X_train.GetCols(), 1);
        for (size_t j = 0; j < X_train.GetCols(); ++j)
        {
            curW[j][0] = dist(rnd);
        }
        if ((curSumErr = AE(X_train * curW, y_train)) < minSumErr)
        {
            minSumErr = curSumErr;
            minW = curW;
        }
    } 
    return minW;
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