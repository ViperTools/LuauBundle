#pragma once
#include <string>

namespace LuaBundle {
    std::string moduleSource = R"V0G0N(local LuaBundle = {
    modules = {}
}

function LuaBundle.Error(formatString, ...)
    error(formatString:format(...))
end

function LuaBundle.Assert(condition, formatString, ...)
    if not condition then
        LuaBundle.Error(formatString, ...)
    end
end

function LuaBundle.RegisterModule(path, closure)
    LuaBundle.modules[path] = closure
end

function LuaBundle.LoadModule(path)
    local func = LuaBundle.modules[path]
    LuaBundle.Assert(func, "Module with path '%s' does not exist or was not caught by the bundler", path)

    local result = { pcall(func) }

    if not result[1] then
        local errorLine, err = result[2]:match(":(%d+):%s*(.+)")
        local lineDefined = debug.getinfo(func).linedefined

        LuaBundle.Error("[LuaBundle] Error in module %s at line %d: %s", path, errorLine - lineDefined, err)
    end

    return select(2, table.unpack(result))
end

)V0G0N";
}