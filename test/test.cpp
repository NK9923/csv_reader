#include <iostream>
#include "Header.h"
#include "variant"

int main() {
    using CSVValue = std::variant<int, double, std::string>;

    size_t csvLineCount;
    bool skipFirstColumn = true;
    auto data12 = csv_Reader::readcsv("test.csv", csvLineCount, false);

    csv_Reader::getwd();

    std::vector<std::vector<CSVValue>> data = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    std::vector<std::string> columnNames = { "Column1", "Column2", "Column3", "Column4"};
    csv_Reader::write_csv(data, "example.csv", true, ',', columnNames);

    std::vector<double> data1 = { 1,2,3,4,5,6,6 };
    csv_Reader::write_csv(data1, "test.csv");

    auto data3 = csv_Reader::readcsv("test.csv", csvLineCount, false);
    csv_Reader::print_data(data3, 4);

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}