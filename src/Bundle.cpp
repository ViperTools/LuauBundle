#include <luau/Parser.h>
#include <RequireVisitor.hpp>
#include <Bundle.hpp>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;

namespace LuaBundle {
    Bundle Bundle::BundleFile(const std::filesystem::path& path, const BundleOptions& options) {
        if (!path.has_filename()) {
            throw std::invalid_argument("Path does not have a file name");
        }
        else if (!fs::exists(path)) {
            throw std::invalid_argument("Path does not exist");
        }

        Bundle bundle(path, options);

        Luau::Allocator allocator;
        Luau::AstNameTable names(allocator);
        Luau::ParseResult parseResult = Luau::Parser::parse(bundle.source.c_str(), bundle.source.length(), names, allocator);

        if (!parseResult.errors.empty()) {
            for (Luau::ParseError error : parseResult.errors) {
                std::cerr << error.getMessage() << std::endl;
            }

            return bundle;
        }

        RequireVisitor visitor(bundle);
        parseResult.root->visit(&visitor);
        bundle.Build();

        return bundle;
    }

    void Bundle::ReplaceRequire(unsigned int line, unsigned int column) {
        size_t lineIndex = -1;

        for (unsigned int i = 0; i < line; i++) {
            lineIndex = source.find('\n', lineIndex + 1);
        }

        source.replace(lineIndex + 1 + column + (lineOffsets[line] * 13), 7, "LuaBundle.LoadModule"); // (lineOffsets[line] * 13) is because "LuaBundle.LoadModule".Length - "require".Length is 13
        lineOffsets[line]++;
    }

    void Bundle::Build() {
        std::set<Module>::reverse_iterator module;

        for (module = modules.rbegin(); module != modules.rend(); module++) {
            std::string src = BundleFile(module->path).source;

            source = "LuaBundle.RegisterModule(\"" + module->name + "\", function()\n" + (options.Tab ? Util::Tab(src) : src) + "\nend)\n\n" + source;
        }
    }
}