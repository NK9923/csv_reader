#include <iostream>
#include "Header.h"
#include "variant"

int main() {
    using CSVValue = std::variant<int, double, std::string>;

    csv_Reader::getwd();

    size_t csvLineCount;
    bool skipFirstColumn = true; // Set to true if you want to skip the first column
    std::vector<std::vector<CSVValue>> csvData = csv_Reader::readcsv("test.csv", csvLineCount, skipFirstColumn);
    csv_Reader::print_data(csvData, 4);
    return 0;
}