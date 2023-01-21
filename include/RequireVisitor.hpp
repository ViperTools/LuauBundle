#pragma once
#include <luau/Ast.h>
#include <Bundle.hpp>
#include <iostream>
#include <filesystem>

namespace LuaBundle {
    struct RequireVisitor : Luau::AstVisitor
    {
        LuaBundle::Bundle& bundle;

        RequireVisitor(LuaBundle::Bundle& bundle)
            : bundle(bundle)
        {}

        bool visit(Luau::AstNode* node) override
        {
            return true;
        }

        bool visit(Luau::AstExprCall* node) override {
            Luau::AstExprGlobal* global = node->func->as<Luau::AstExprGlobal>();

            if (!global || strcmp(global->name.value, "require")) {
                return true;
            }

            if (Luau::AstExprConstantString* constString = node->args.data[0]->as<Luau::AstExprConstantString>()) {
                std::string str(constString->value.data);

                if (!str.ends_with(".lua") && !str.ends_with(".luau")) {
                    return true;
                }

                std::filesystem::path path = bundle.path.parent_path().append(str);

                if (std::filesystem::exists(path)) {
                    bundle.ReplaceRequire(global->location.begin.line, global->location.begin.column);
                    bundle.modules.emplace(str, path);
                }
            }

            return true;
        }
    };
}