import('smake/gpp', true)

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
    standard('gnu++17')
    include {
        '../Ast/include',
        '../Common/include'
    }
    input('../Ast/src/*.cpp')
    flags('-c')

    local outDir = ('dependencies/luau/out'):format(name)
    run(
        'rm -rf ' .. outDir,
        'mkdir ' .. outDir
    )
    runIn(outDir, makeCommand())
end
