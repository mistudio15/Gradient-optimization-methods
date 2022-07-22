#pragma once


#include "stdafx.h"
#include "matrix.h"

using std::cout;
using std::endl;

double FindExtremum(std::function<double(double)> const &df, std::function<double(double const &, double const &)> const &BinOp, double eps = 0.001);

class LinearModel
{
public:
    LinearModel(double lr_ = 0.00010376) : lr(lr_) {};
    virtual void Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) = 0;
    virtual Linalg::Matrix<double> Predict(Linalg::Matrix<double> const &X_test) const = 0;
protected:
    double lr;
    Linalg::Matrix<double> w;
    
    Linalg::Matrix<double> WeightsMinErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train);
    double SumErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train, Linalg::Matrix<double> const &testW);
};

class GradientDescent : public LinearModel
{
public:
    GradientDescent(double lr_ = 0.00010376) : LinearModel(lr_) {};
    void Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) override;
    Linalg::Matrix<double> Predict(Linalg::Matrix<double> const &X_test) const override;
};


class SGD : public LinearModel
{
public:
    SGD(double lr_ = 0.00010376) : LinearModel(lr_) {};
    void Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train);
    Linalg::Matrix<double> Predict(Linalg::Matrix<double> const &X_test) const;
private:
};

