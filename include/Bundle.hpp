#pragma once
#include <Bundle.hpp>
#include <Util.hpp>
#include <filesystem>
#include <string>
#include <set>
#include <unordered_map>

#include <iostream>

namespace LuaBundle {
    struct Module {
        Module(std::string name, std::string path)
            : name(name),
            path(path)
        {}

        std::string name;
        std::filesystem::path path;

        bool operator <(const Module &other) const {
            return name < other.name;
        }
    };

    struct BundleOptions {
        bool Tab = true;
    };

    struct Bundle {
        Bundle(std::filesystem::path path, const BundleOptions& options = {})
            : path(path),
            source(Util::ReadFile(path.string())),
            options(options)
        {}

        static Bundle BundleFile(const std::filesystem::path& path, const BundleOptions& options = {});
        void ReplaceRequire(unsigned int line, unsigned int column);
        void Build();

        const BundleOptions& options;
        std::unordered_map<unsigned int, int> lineOffsets;
        std::filesystem::path path;
        std::string source;
        std::set<Module> modules;
    };
}