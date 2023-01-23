#pragma once
#include <string>
#include <Util.hpp>

namespace LuauBundle {
    static std::string moduleSource = R"V0G0N(local LuauBundle = {
    modules = {},
    cache = {},
    lineMap = {}
}

function LuauBundle.RegisterModule(path, closure)
    LuauBundle.modules[path] = closure
end

function LuauBundle.LoadModule(path)
    local func = LuauBundle.modules[path]

    if not func then
        error(("Module with path '%s' does not exist or was not caught by the bundler"):format(path))
    end

    if LuauBundle.cache[path] then
        return table.unpack(LuauBundle.cache[path])
    end

    local result = { func() }
    LuauBundle.cache[path] = result

    return table.unpack(LuauBundle.cache[path])
end

function LuauBundle.GetFilesFromLine(line)
    local files = {}

    for name, location in next, LuauBundle.lineMap do
        if line > location[1] and line < location[2] then
            table.insert(files, name)
        end
    end

    table.sort(files, function(a, b)
        return LuauBundle.lineMap[a][1] > LuauBundle.lineMap[b][1]
    end)

    return files
end

function LuauBundle.TraceError(line, message)
    local files = LuauBundle.GetFilesFromLine(line)

    if #files == 0 then
        return;
    end

    local error = ("%s:%d: %s"):format(files[1], line - LuauBundle.lineMap[files[1]][1], message)

    for i = 2, #files do
        error = error .. "\nrequired from " .. files[i]
    end

    return error
end

if LuauBundle_errorConnection then
    LuauBundle_errorConnection:Disconnect()
end

getgenv().LuauBundle_errorConnection = game:GetService("ScriptContext").ErrorDetailed:Connect(function(message)
    local line, message = message:match('.+:(%d+):%s*(.+)')
    warn('[LuauBundle] ' .. LuauBundle.TraceError(tonumber(line), message))
end)

)V0G0N";

    static const size_t moduleSourceLineCount = Util::CountLines(moduleSource);
}