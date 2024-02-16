#pragma once

#define EXPORT

#ifdef EXPORT
#define dllimportexport dllexport
#else
#define dllimportexport dllimport
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <variant>

namespace csv_Reader {
    __declspec(dllimportexport) std::vector<std::vector<std::variant<int, double, std::string>>> readcsv(const std::string& file_name, size_t& csvLineCount, bool skipFirstColumn, char delimiter);
    __declspec(dllimportexport) void print_data(std::vector<std::vector< std::variant<int, double, std::string>>>& data, int precision);
    __declspec(dllimportexport) std::string trim(const std::string& s);
    __declspec(dllimportexport) int getLength(const char* filename);
    __declspec(dllimportexport) std::vector<std::string> getWeekdayDates(int startYear, int endYear, const std::string& startDateStr, const std::string& endDateStr);
    __declspec(dllimportexport) void getwd();
}
