#include <luau/Parser.h>
#include <RequireVisitor.hpp>
#include <Bundle.hpp>
#include <fstream>
#include <iostream>

namespace LuaBundle {
    Bundle::Bundle(const std::filesystem::path& path, const BundleOptions& options, const Bundle* parent)
        : path(path),
        source(Util::ReadFile(path.string())),
        options(options),
        parent(parent)
    {
        if (!path.has_filename()) {
            throw std::invalid_argument("Path does not have a file name");
        }
        else if (!std::filesystem::exists(path)) {
            throw std::invalid_argument("Path does not exist");
        }

        Luau::Allocator allocator;
        Luau::AstNameTable names(allocator);
        Luau::ParseResult parseResult = Luau::Parser::parse(source.c_str(), source.length(), names, allocator);

        if (!parseResult.errors.empty()) {
            for (Luau::ParseError error : parseResult.errors) {
                std::cerr << error.getMessage() << std::endl;
            }

            return;
        }

        RequireVisitor visitor(this);
        parseResult.root->visit(&visitor);
        BuildSource();
    }

    bool Bundle::ContainsPath(const std::string& relativePath) const {
        if (modules.contains(relativePath)) {
            return true;
        }
        else if (parent) {
            return parent->ContainsPath(relativePath);
        }

        return false;
    }

    void Bundle::Require(Luau::AstExprGlobal* global, Luau::AstExprConstantString* argument, const std::filesystem::path& path) {
        // Replace require with LuaBundle.LoadModule
        size_t line = global->location.begin.line;
        Util::ReplaceOnLine(source, line, global->location.begin.column + lineOffsets[line], 7, "LuaBundle.LoadModule");
        lineOffsets[line] += 13;

        // Replace argument with proper name
        std::string name = path.string();
        line = argument->location.begin.line;
        Util::ReplaceOnLine(source, line, argument->location.begin.column + lineOffsets[line], argument->value.size + 2, '"' + name + '"');
        lineOffsets[line] += name.size() - argument->value.size;

        modules.insert(name);
    }

    void Bundle::BuildSource() {
        std::string header;

        for (const std::string& path : modules) {
            if (parent && parent->ContainsPath(path)) {
                continue;
            }

            std::string bundleSource = Bundle(path, { .line = line + 1 }, this).source;
            std::string registerSource = "LuaBundle.RegisterModule(\"" + path + "\", function()\n" + (options.Tab ? Util::Tab(bundleSource) : bundleSource) + "\nend)\n\n";

            size_t lineCount = Util::CountLines(registerSource);

            // Store locations in root bundle

            if (!parent) {
                lines[path] = { line, line + lineCount - 2 };
            }
            else {
                const Bundle* root = parent;

                while (root->parent) {
                    root = root->parent;
                }

                root->lines[path] = { line, line + lineCount - 2 };
            }

            line += lineCount;

            header = header + registerSource;
        }

        source = header + source;
    }
    
    std::string Bundle::BuildLineMap() {
        std::string map = "{";

        for (const auto& [path, location] : lines) {
            map += "[\"" + path + "\"]={" + std::to_string(location.startLine) + "," + std::to_string(location.endLine) + "},";
        }

        return map.replace(map.size() - 1, map.size() - 1, 1, '}');
    }
}