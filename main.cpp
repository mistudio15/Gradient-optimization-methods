#include "matrix.h"
#include "algorithms.h"

/*
    Инсайт: рано говорить про оценку скорости работы (при использовании экспоненц. скольз. среднего),
    потому что выборка содержит 100 объектов
    Стоит загрузить выборку, где нужно оценить стоимость жилья из курса ML
    Тогда можно уже рассматривать скорость работы при использовании эксп. скольз. среднего и др эвристик
*/


/*
    - Sum Mean Sqr сделать по столбцам, а не в общем
    - MSE MAE должно принимать y_test и y_pred
*/

/*
    Задачи
    + инициализация матрицы через итераторы
    + Copy через вектор (мб не Copy)
    + операторы для векторов внести внутрь операторов для матрицы
    - реализовать fill со значениями генерируемыми функтором 
    + Добавление столбца для свободного члена
        + Методы добавления столбца и строки 
    - случайный выбор B объектов из выбоки (сейчас по очереди)
        - рассмотреть эвристики выбора объектов из выборки (лекции)
    - Скользящее среднее для градиента 
    - Эвристики для SGD
    - word2vec

*/

/*
    Test dataset excel
    GD   lr = 0.00010376
    SGD  lr = 0.00000451
*/

/*
    numeric_features
    numeric_scaled
    ohe_scaled
    ohe_scaled_perc
*/

int main(int argc, char *argv[])
{   

    std::ifstream file_x_train("../data/numeric_scaled/x_train.csv");
    std::ifstream file_y_train("../data/numeric_scaled/y_train.csv");
    std::ifstream file_x_test("../data/numeric_scaled/x_test.csv");
    std::ifstream file_y_test("../data/numeric_scaled/y_test.csv");
    if (!file_x_train || !file_y_train || !file_x_test || !file_y_test)
    {
        return 0;
    }
    Linalg::Matrix<double> X_train = Linalg::ReadCSV(file_x_train);
    Linalg::Matrix<double> y_train = Linalg::ReadCSV(file_y_train);
    Linalg::Matrix<double> X_test = Linalg::ReadCSV(file_x_test);
    Linalg::Matrix<double> y_test = Linalg::ReadCSV(file_y_test);

    std::random_device rd;
    std::mt19937 rdnX(rd());
    std::mt19937 rdnY(rdnX);
    std::mt19937 rdnXtest(rdnX);
    std::mt19937 rdnYtest(rdnX);


    std::shuffle(X_train.IterRow(0), X_train.IterRow(X_train.GetRows()), rdnX);
    std::shuffle(y_train.IterRow(0), y_train.IterRow(y_train.GetRows()), rdnY);
    std::shuffle(X_test.IterRow(0), X_test.IterRow(X_test.GetRows()), rdnXtest);
    std::shuffle(y_test.IterRow(0), y_test.IterRow(y_test.GetRows()), rdnYtest);

    // без этого действия numeric_scaled выдавало MAE = 1e5
    // с ним сразу 2.4e4
    X_train.AddCol(1);

    SGD model(0.01);
    model.Fit(X_train, y_train);

    X_test.AddCol(1);
    std::cout << std::setw(20) << "MAE test = " << MAE(y_test, model.Predict(X_test)) << std::endl;

    


    // Linalg::Matrix<double> y_pred = model.Predict({{16, 79, 0, 1}});
    // y_pred.Show();

    





    // std::function<double(double)> df = [](double x) -> double
    // {
    //     return 4 * x * x * x - 12 * x * x + 4;
    // };
    // double x_min = FindExtremum(df, std::minus<double>());
    // std::cout << "x_min: " << x_min << std::endl;
}