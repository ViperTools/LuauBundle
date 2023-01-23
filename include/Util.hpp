#pragma once
#include <string>
#include <fstream>
#include <sstream>

namespace LuauBundle::Util {
    static std::string ReadFile(const std::string& path) {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();

        return std::move(buffer).str();
    }

    static std::string& Tab(std::string& str) {
        str = '\t' + str;
        size_t index = str.find('\n');

        while (index != std::string::npos) {
            size_t temp = index;
            index = str.find('\n', index + 1);

            if (index - temp > 1) { // Don't tab empty lines
                str.replace(temp, 1, "\n\t");
            }
        }

        return str;
    }

    static bool Replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);

        if(start_pos == std::string::npos)
            return false;

        str.replace(start_pos, from.length(), to);

        return true;
    }

    static std::string& ReplaceOnLine(std::string& string, size_t line, int column, int length, std::string replacement) {
        size_t lineIndex = -1;

        for (unsigned int i = 0; i < line; i++) {
            lineIndex = string.find('\n', lineIndex + 1);
        }

        string.replace(lineIndex + 1 + column, length, replacement); 

        return string;
    }

    static size_t CountLines(const std::string& str) {
        return std::count(str.begin(), str.end(), '\n') + (!str.empty() && str.back() != '\n');
    }
}