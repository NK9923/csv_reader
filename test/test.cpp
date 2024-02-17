#include <iostream>
#include "Header.h"
#include "variant"

int main() {
    using CSVValue = std::variant<int, double, std::string>;

    csv_Reader::getwd();

    size_t csvLineCount;
    bool skipFirstColumn = true;
    std::vector<std::vector<CSVValue>> csvData = csv_Reader::readcsv("test.csv", csvLineCount, skipFirstColumn);
    csv_Reader::print_data(csvData, 4);

    std::vector<std::vector<CSVValue>> data = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    std::vector<std::string> columnNames = { "Column1", "Column2", "Column3", "Column4"};
    csv_Reader::write_csv(data, "example.csv", true, ',', columnNames);

    return 0;
}