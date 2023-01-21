#pragma once
#include <string>
#include <fstream>
#include <sstream>

namespace LuaBundle::Util {
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
}