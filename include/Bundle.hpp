#pragma once
#include <Bundle.hpp>
#include <LuaModule.hpp>
#include <Util.hpp>
#include <filesystem>
#include <string>
#include <set>
#include <unordered_map>

#include <iostream>

namespace LuaBundle {
    struct BundleOptions {
        bool Tab = true;
        size_t line = moduleSourceLineCount + 1;
    };

    struct ModuleLocation {
        size_t startLine, endLine;
    };

    struct Bundle {
        Bundle(const std::filesystem::path& path, const BundleOptions& options = {}, const Bundle* parent = 0);

        bool ContainsPath(const std::string& relativePath) const;
        void Require(Luau::AstExprGlobal* global, Luau::AstExprConstantString* argument, const std::filesystem::path& path);
        void BuildSource();
        std::string BuildLineMap();
        void BuildRoot();

        const Bundle* parent;
        const BundleOptions& options;
        std::filesystem::path path;
        std::string source;
        std::set<std::string> modules;
        size_t line = options.line;
        mutable std::unordered_map<std::string, ModuleLocation> lines;
        std::unordered_map<unsigned int, int> lineOffsets;
    };
}