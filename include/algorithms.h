#pragma once


#include "stdafx.h"
#include "matrix.h"

using std::cout;
using std::endl;

double FindExtremum(std::function<double(double)> const &df, std::function<double(double const &, double const &)> const &BinOp, double eps = 0.001);

double AE(Linalg::Matrix<double> const &y, Linalg::Matrix<double> const &y_pred);

Linalg::Matrix<double> WeightsMinErr(Linalg::Matrix<double> const &X_train, Linalg::Matrix<double> const &y_train);

double MSE(Linalg::Matrix<double> const &y, Linalg::Matrix<double> const &y_pred);

double MAE(Linalg::Matrix<double> const &y, Linalg::Matrix<double> const &y_pred);

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

