#pragma once
#include <string>
#include <Util.hpp>

namespace LuaBundle {
    static std::string moduleSource = R"V0G0N(local LuaBundle = {
    modules = {},
    cache = {},
    lineMap = {}
}

function LuaBundle.RegisterModule(path, closure)
    LuaBundle.modules[path] = closure
end

function LuaBundle.LoadModule(path)
    local func = LuaBundle.modules[path]

    if not func then
        error(("Module with path '%s' does not exist or was not caught by the bundler"):format(path))
    end

    if LuaBundle.cache[path] then
        return table.unpack(LuaBundle.cache[path])
    end

    local result = { func() }
    LuaBundle.cache[path] = result

    return table.unpack(LuaBundle.cache[path])
end

function LuaBundle.GetFilesFromLine(line)
    local files = {}

    for name, location in next, LuaBundle.lineMap do
        if line > location[1] and line < location[2] then
            table.insert(files, name)
        end
    end

    table.sort(files, function(a, b)
        return LuaBundle.lineMap[a][1] > LuaBundle.lineMap[b][1]
    end)

    return files
end

function LuaBundle.TraceError(line, message)
    local files = LuaBundle.GetFilesFromLine(line)

    if #files == 0 then
        return;
    end

    local error = ("%s:%d: %s"):format(files[1], line - LuaBundle.lineMap[files[1]][1], message)

    for i = 2, #files do
        error = error .. "\nrequired from " .. files[i]
    end

    return error
end

if LuaBundle_errorConnection then
    LuaBundle_errorConnection:Disconnect()
end

getgenv().LuaBundle_errorConnection = game:GetService("ScriptContext").ErrorDetailed:Connect(function(message)
    local line, message = message:match('.+:(%d+):%s*(.+)')
    warn('[LuaBundle] ' .. LuaBundle.TraceError(tonumber(line), message))
end)

)V0G0N";

    static const size_t moduleSourceLineCount = Util::CountLines(moduleSource);
}