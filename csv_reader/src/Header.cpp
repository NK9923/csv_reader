#include "pch.h"
#include "Header.h"
#include <iostream>


using CSVValue = std::variant<int, double, std::string>;

std::string csv_Reader::trim(const std::string& s) {
    size_t start = 0;
    size_t end = s.length();

    while (start < end && (std::isspace(s[start]) || !std::isalnum(s[start]))) {
        start++;
    }

    while (end > start && (std::isspace(s[end - 1]) || !std::isalnum(s[end - 1]))) {
        end--;
    }

    return s.substr(start, end - start);
};


std::vector<std::string> csv_Reader::getWeekdayDates(int startYear, int endYear, const std::string& startDateStr, const std::string& endDateStr) {
    std::vector<std::string> weekdayDates;

    std::tm startDateInfo = {};
    std::istringstream(startDateStr) >> std::get_time(&startDateInfo, "%Y-%m-%d");

    std::tm endDateInfo = {};
    std::istringstream(endDateStr) >> std::get_time(&endDateInfo, "%Y-%m-%d");

    for (int year = startYear; year <= endYear; ++year) {
        for (int month = 1; month <= 12; ++month) {
            std::tm timeinfo = {};
            timeinfo.tm_year = year - 1900; // Years since 1900
            timeinfo.tm_mon = month - 1;   // Months are zero-based
            timeinfo.tm_mday = 1;          // First day of the month

            while (timeinfo.tm_mon == month - 1) {
                // day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
                std::mktime(&timeinfo);
                int dayOfWeek = timeinfo.tm_wday;

                // Check if the current day is a weekday (Monday to Friday)
                if (dayOfWeek >= 1 && dayOfWeek <= 5) {
                    char buffer[11];
                    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
                    std::tm currentDateInfo = timeinfo;
                    if (std::mktime(&currentDateInfo) >= std::mktime(&startDateInfo) &&
                        std::mktime(&currentDateInfo) <= std::mktime(&endDateInfo)) {
                        weekdayDates.emplace_back(buffer);
                    }
                    if (std::mktime(&timeinfo) == std::mktime(&endDateInfo)) {
                        return weekdayDates;
                    }
                }
                timeinfo.tm_mday++;
            }
        }
    }
    return weekdayDates;
}


void csv_Reader::getwd() {
    std::filesystem::path currentPath = std::filesystem::current_path();

    // Print the current working directory
    std::cout << "Current Working Directory: " << currentPath << std::endl;
}



int csv_Reader::getLength(const char* filename) {
    int numRows = 0;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return -1;
    }
    std::string line;
    while (std::getline(file, line)) {
        numRows++;
    }
    return numRows;
};


std::vector<std::vector<CSVValue>> csv_Reader::readcsv(const std::string& file_name, size_t& csvLineCount, bool skipFirstColumn, char delimiter) {
    std::ifstream fin(file_name);

    if (delimiter != ',' && delimiter != ';') {
        throw std::invalid_argument("Invalid delimiter. Supported delimiters are ',' and ';'");
    }

    if (!fin.is_open()) {
        throw std::runtime_error("Error opening file: " + file_name);
    }

    std::vector<std::vector<CSVValue>> lines;

    std::string line;
    if (std::getline(fin, line)) { 
        std::istringstream iss(line);
        std::string token;
        std::vector<CSVValue> columns;

        bool firstColumnSkipped = false;

        while (std::getline(iss, token, delimiter)) {
            if (!firstColumnSkipped && skipFirstColumn) {
                firstColumnSkipped = true;
                continue;
            }
            columns.push_back(CSVValue{}); 
        }
        lines.push_back(columns);  
    }

    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<CSVValue> columns;

        bool firstColumnSkipped = false;

        while (std::getline(iss, token, delimiter)) {
            if (!firstColumnSkipped && skipFirstColumn) {
                firstColumnSkipped = true;
                continue;
            }

            std::istringstream tokenStream(token);
            int intValue;
            double doubleValue;

            if (tokenStream >> doubleValue) {
                columns.push_back(doubleValue);
            }
            else {
                if (tokenStream >> intValue) {
                    columns.push_back(intValue);
                }
                else {
                    columns.push_back(token);
                }
            }
        }
        lines.push_back(columns);
    }
    csvLineCount = lines.size();

    fin.close(); 
    return lines;
}


void csv_Reader::print_data(std::vector<std::vector<CSVValue>>& data, int precision) {
    for (const auto& row : data) {
        for (const auto& col : row) {
            std::visit([precision](const auto& value) {
                std::cout << std::fixed << std::setw(4 + precision) << std::setprecision(precision) << value << "\t";
                }, col);
        }
        std::cout << std::endl;
    }
    std::cout << std::defaultfloat; 
}


void csv_Reader::write_csv(std::vector<std::vector<CSVValue>>& data, std::string filename, bool index, char delimiter, std::optional<std::vector<std::string>> columnNames) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open the file for writing." << std::endl;
        return;
    }

    if (columnNames && !columnNames->empty()) {
        for (size_t i = 0; i < columnNames->size(); ++i) {
            outputFile << (*columnNames)[i];
            if (i < columnNames->size() - 1) {
                outputFile << delimiter;
            }
        }
        outputFile << std::endl;
    }

    for (size_t i = 0; i < data.size(); ++i) {
        if (index) {
            outputFile << i + 1 << ",";
        }

        for (size_t j = 0; j < data[i].size(); ++j) {
            const CSVValue& value = data[i][j];
            std::visit([&outputFile](auto&& arg) {
                outputFile << arg;
                }, value);

            if (j < data[i].size() - 1) {
                outputFile << delimiter;
            }
        }
        outputFile << std::endl;
    }
    outputFile.close();
};