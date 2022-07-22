#include "algorithms.h"
#include "matrix.h"


double LinearModel::SumErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train, Linalg::Matrix<double> const &testW)
{
    return (X_train * testW - y_train).Abs().Sum();
}

Linalg::Matrix<double> LinearModel::WeightsMinErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train)
{
    std::random_device rd;
    std::mt19937 rnd(rd());
    std::uniform_real_distribution<> dist(-2000, 2000);
    Linalg::Matrix<double> minW(X_train.GetCols(), 1); // fill 0
    double minSumErr = SumErr(X_train, y_train, minW);
    for (size_t i = 0; i < 1000; ++i)
    {
        Linalg::Matrix<double> curW(X_train.GetCols(), 1);
        for (size_t j = 0; j < X_train.GetCols(); ++j)
        {
            curW[j][0] = dist(rnd);
        }
        if (SumErr(X_train, y_train, curW) < minSumErr)
        {
            minW = curW;
        }
    } 
    return minW;
}


template <class T>
int sign(T value)
{
    return value > 0 ? 1 : -1; 
}

void GradientDescent::Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) 
{
    assert(X_train.GetRows() == y_train.GetRows());
    w = WeightsMinErr(X_train, y_train);
    double originalErr = SumErr(X_train, y_train, w);
    std::cout << originalErr << std::endl;
    // double end = 10000;
    // double start = 1000;
    // double clr = 0.0001;
    // double rateLr = 10000;
    Linalg::Matrix<double> f;
    Linalg::Matrix<double> err;
    Linalg::Matrix<double> prevErr;
    Linalg::Matrix<double> grad;
    int N = 1000000;
    size_t i = 0;
    for (; i < N; ++i)
    {
        // lr = std::max(clr * std::exp(((double)i + 1) / rateLr), 0.00001);
        // lr = clr * std::log10(10 + 0.0001 * i);
        // lr = 1 / std::min(double(i + start), end);
        // std::cout << "\t\t\t\tlr = " << lr << std::endl;

        f = X_train * w;
        err = f - y_train;
        grad = (2 * (X_train.T() * err)) / X_train.GetRows();
        w -= lr * grad;

        
        // grad.Show("grad___________");
        std::cout << "err = " << err.Abs().Sum() << std::endl;
        // если текущая ошибка в 100 раз больше чем исходная, завершаем
        if (err.Abs().Sum() > originalErr * 100)
        {
            
            break;
        }
        // если 100 итераций назад была такая же ошибка, завершаем
        if (i % 100 == 0)
        {
            if (prevErr.Abs().Sum() == err.Abs().Sum())
            {
                break;
            }
            prevErr = err;
        }
        // sleep(1);
    }
    w.Show("\t\t-----------------------w");
    std::cout << "SumErr = " << err.Abs().Sum() / 1000 << std::endl;
    std::cout << "original SumErr = " << originalErr / 1000 << std::endl; 
    std::cout << "Itarations = " << i << std::endl;
}

Linalg::Matrix<double> GradientDescent::Predict(Linalg::Matrix<double> const &X_test) const
{
    assert(X_test.GetCols() == w.GetRows());
    w.Show();
    return X_test * w;
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