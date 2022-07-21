#include <iostream>
#include <functional>
#include <fstream>

#include "matrix.h"
#include "algorithms.h"

int main(int argc, char *argv[])
{
//     Linalg::Matrix<double> X_train = {
//         {
//             {	10	,	70	,	1	},
//             {	5	,	30	,	0	},
//             {	3	,	70	,	1	},
//             {	23	,	50	,	1	},
//             {	12	,	60	,	1	},
//             {	7	,	70	,	1	},
//             {	11	,	80	,	0	},
//             {	24	,	90	,	1	},
//             {	24	,	40	,	0	},
//             {	11	,	50	,	1	},
//             {	20	,	70	,	0	},
//             {	2	,	50	,	1	},
//             {	5	,	60	,	1	},
//             {	7	,	90	,	0	},
//             {	6	,	40	,	1	},
//             {	15	,	40	,	1	},
//             {	6	,	30	,	0	},
//             {	8	,	120	,	1	},
//             {	21	,	60	,	1	},
//             {	3	,	60	,	0	}	
//         }
//     };

//     Linalg::Matrix<double> y_train = {
//         {
//             {	63000	}	,
//             {	25000	}	,
//             {	70000	}	,
//             {	30000	}	,
//             {	51000	}	,
//             {	66000	}	,
//             {	69000	}	,
//             {	69000	}	,
//             {	16000	}	,
//             {	42000	}	,
//             {	50000	}	,
//             {	51000	}	,
//             {	58000	}	,
//             {	83000	}	,
//             {	37000	}	,
//             {	28000	}	,
//             {	24000	}	,
//             {	115000	}	,
//             {	42000	}	,
//             {	57000	}	
	

//         }
//     };

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
    X_train.Show();
    y_train.Show();
    

    GradientDescent model;
    model.Fit(X_train, y_train);


    // Linalg::Matrix<double> y_pred = model.Predict({{{16, 79, 0}}});
    // y_pred.Show();
    





    // std::function<double(double)> df = [](double x) -> double
    // {
    //     return 4 * x * x * x - 12 * x * x + 4;
    // };
    // double x_min = FindExtremum(df, std::minus<double>());
    // std::cout << "x_min: " << x_min << std::endl;
}