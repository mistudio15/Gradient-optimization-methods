#pragma once


#include "stdafx.h"
#include "matrix.h"

using std::cout;
using std::endl;

double FindExtremum(std::function<double(double)> const &df, std::function<double(double const &, double const &)> const &BinOp, double eps = 0.001);

double SumErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train, Linalg::Matrix<double> const &testW);

Linalg::Matrix<double> WeightsMinErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train);

double MSE(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train, Linalg::Matrix<double> const &w);

double MAE(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train, Linalg::Matrix<double> const &w);

class LinearModel
{
public:
    LinearModel(double lr_) : lr(lr_) {};
    virtual void Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) = 0;
    virtual Linalg::Matrix<double> Predict(Linalg::Matrix<double> const &X_test) const = 0;
protected:
    double lr;
    Linalg::Matrix<double> w;
};

class GradientDescent : public LinearModel
{
public:
    GradientDescent(double lr_ = 0.001) : LinearModel(lr_) {};
    void Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) override;
    Linalg::Matrix<double> Predict(Linalg::Matrix<double> const &X_test) const override;
};


class SGD : public LinearModel
{
public:
    SGD(double lr_ = 0.001) : LinearModel(lr_) {};
    void Fit(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train) override;
    Linalg::Matrix<double> Predict(Linalg::Matrix<double> const &X_test) const override;
private:
};

