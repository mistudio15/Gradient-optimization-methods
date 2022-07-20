#pragma once

#include <functional>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "matrix.h"

using std::cout;
using std::endl;

double FindExtremum(std::function<double(double)> const &df, std::function<double(double const &, double const &)> const &BinOp, double eps = 0.001);

class GradientDescent
{
public:
    GradientDescent(double lr_ = 0.0001) : lr(lr_) {};
    void Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train);
    Linalg::Matrix<double> Predict(Linalg::Matrix<double> const &X_test) const;
private:
    double lr;
    Linalg::Matrix<double> w;
};

