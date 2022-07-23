#include "matrix.h"
#include "algorithms.h"

/*
    Инсайт: рано говорить про оценку скорости работы (при использовании экспоненц. скольз. среднего),
    потому что выборка содержит 100 объектов
    Стоит загрузить выборку, где нужно оценить стоимость жилья из курса ML
    Тогда можно уже рассматривать скорость работы при использовании эксп. скольз. среднего и др эвристик
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


int main(int argc, char *argv[])
{
    std::ifstream file("../" + std::string(argv[1]));
    if (!file)
    {
        return 0;
    }
    int n, d;
    file >> n >> d;
    Linalg::Matrix<double> X_train(n, d);
    Linalg::Matrix<double> y_train(n, 1);
    size_t i = 0;
    while (!file.eof())
    {
        for (size_t j = 0; j < d; ++j)
        {
            file >> X_train[i][j];
        }
        file >> y_train[i][0];
        ++i;
    }

    X_train.AddCol(1);

    SGD model(0.00000451);
    // GradientDescent model(0.00010376);
    model.Fit(X_train, y_train);


    Linalg::Matrix<double> y_pred = model.Predict({{16, 79, 0, 1}});
    y_pred.Show();
    





    // std::function<double(double)> df = [](double x) -> double
    // {
    //     return 4 * x * x * x - 12 * x * x + 4;
    // };
    // double x_min = FindExtremum(df, std::minus<double>());
    // std::cout << "x_min: " << x_min << std::endl;
}