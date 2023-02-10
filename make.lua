---@diagnostic disable: undefined-global
using('g++')

local dependencies = { 'luau' }

function smake.build()
    -- Main
    standard('c++2a')
    include { 'include', 'dependencies' }
    input('main.cpp', 'src/*.cpp')
    output(platform.is_windows and 'luau-bundle.exe' or 'luau-bundle')

    -- Luau Parser
    include('dependencies/luau/Common/include')
    include('dependencies/luau/Ast/include')
    input('dependencies/luau/out/*.o')

    if platform.is_osx then
        flags('-framework CoreFoundation -framework CoreServices')
    elseif platform.is_linux then
        flags('-pthread')
    end

    build()
end

function smake.run()
    run(platform.is_windows and './luau-bundle.exe' or './luau-bundle')
end

function smake.install()
    for _, name in next, dependencies do
        local outDir = ('dependencies/%s/out'):format(name)

        run(
            'rm -rf ' .. outDir,
            'mkdir ' .. outDir,
            ('cd %s && smake -f../make.lua'):format(outDir)
        )
    end
end
