#include <istream>
#include <string>
#include <vector>

enum class CSVState {
    UnquotedField,
    QuotedField,
    QuotedQuote
};

std::vector<std::string> readCSVRow(const std::string &row) {
    CSVState state = CSVState::UnquotedField;
    std::vector<std::string> fields {""};
    size_t i = 0; // index of the current field
    for (char c : row) {
        switch (state) {
            case CSVState::UnquotedField:
                switch (c) {
                    case ',': // end of field
                              fields.push_back(""); i++;
                              break;
                    case '"': state = CSVState::QuotedField;
                              break;
                    default:  fields[i].push_back(c);
                              break; }
                break;
            case CSVState::QuotedField:
                switch (c) {
                    case '"': state = CSVState::QuotedQuote;
                              break;
                    default:  fields[i].push_back(c);
                              break; }
                break;
            case CSVState::QuotedQuote:
                switch (c) {
                    case ',': // , after closing quote
                              fields.push_back(""); i++;
                              state = CSVState::UnquotedField;
                              break;
                    case '"': // "" -> "
                              fields[i].push_back('"');
                              state = CSVState::QuotedField;
                              break;
                    default:  // end of quote
                              state = CSVState::UnquotedField;
                              break; }
                break;
        }
    }
    return fields;
}

/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes"""
std::vector<std::vector<std::string>> readCSV(std::istream &in) {
    std::vector<std::vector<std::string>> table;
    std::string row;
    while (!in.eof()) {
        std::getline(in, row);
        if (in.bad() || in.fail()) {
            break;
        }
        auto fields = readCSVRow(row);
        table.push_back(fields);
    }
    return table;
}





//// _______________________________________-------------------------------------------------------------------------------------


#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{
    public:
        std::string_view operator[](std::size_t index) const
        {
            return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
        }
        std::size_t size() const
        {
            return m_data.size() - 1;
        }
        void readNextRow(std::istream& str)
        {
            std::getline(str, m_line);

            m_data.clear();
            m_data.emplace_back(-1);
            std::string::size_type pos = 0;
            while((pos = m_line.find(',', pos)) != std::string::npos)
            {
                m_data.emplace_back(pos);
                ++pos;
            }
            // This checks for a trailing comma with no data after it.
            pos   = m_line.size();
            m_data.emplace_back(pos);
        }
    private:
        std::string         m_line;
        std::vector<int>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}   
int main()
{
    std::ifstream       file("plop.csv");

    CSVRow              row;
    while(file >> row)
    {
        std::cout << "4th Element(" << row[3] << ")\n";
    }
}