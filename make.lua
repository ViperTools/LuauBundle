---@diagnostic disable: undefined-global
using('g++')

local dependencies = { 'luau' }

function vmake.build()
    -- Main
    standard('c++2a')
    include { 'include', 'dependencies' }
    input('main.cpp', 'src/*.cpp')
    output(platform.is_windows and 'lua-bundle.exe' or 'lua-bundle')

    -- Luau Parser
    include('dependencies/luau/Common/include')
    include('dependencies/luau/Ast/include')
    input('dependencies/luau/out/*.o')

    build()
end

function vmake.run()
    run(platform.is_windows and './lua-bundle.exe' or './lua-bundle')
end

function vmake.install()
    for _, name in next, dependencies do
        local outDir = ('dependencies/%s/out'):format(name)

        run(
            'rm -rf ' .. outDir,
            'mkdir ' .. outDir,
            ('cd %s && vmake -f../make.lua'):format(outDir)
        )
    end
end
