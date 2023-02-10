---@diagnostic disable: undefined-global
using('g++')

function smake.build()
    standard('gnu++17')
    include {
        '../Ast/include',
        '../Common/include'
    }
    input('../Ast/src/*.cpp')
    flags('-c')
    build()
end