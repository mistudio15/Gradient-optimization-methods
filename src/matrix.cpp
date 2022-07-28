#include "matrix.h"

Linalg::Matrix<double> Linalg::ReadCSV(std::istream &file)
{
    Linalg::Matrix<double> dataset;
    int nHeaders;
    {
        std::string features;
        std::getline(file, features);
        // "head1, head2, head3, head4" - 3 commas - 4 headers
        nHeaders = std::count(features.begin(), features.end(), ','); 
        ++nHeaders;
    }
    std::vector<double> rowVec;
    std::stringstream rowStrStream;
    std::string rowStr;
    std::string cell;
    while (std::getline(file, rowStr))
    {
        rowStrStream << rowStr;
        rowVec.clear();

        // Последняя ячейка не отделена запятой, поэтому до nHeaders - 1
        for (size_t i = 0; i < nHeaders - 1; ++i)
        {
            std::getline(rowStrStream, cell, ',');
            rowVec.push_back(std::stof(cell));
        }
        std::getline(rowStrStream, cell);
        rowVec.push_back(std::stof(cell));
     
        dataset.AddRow(rowVec);
        rowStrStream.clear();
    }
    return dataset; 
}