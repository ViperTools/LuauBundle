# LuauBundle
LuauBundle is a Luau bundler that uses the require function to allow you to include other lua files and output one bundle file. By default it is configured for Roblox, but modifying LuaModule.hpp to remove some of the custom error handling will allow it to work with normal Luau.

## Building
LuauBundle uses [vmake](https://github.com/ViperTools/vmake) for building. Run `vmake -i` to build dependencies, then run `vmake -b` to build, or simply run `vmake -bi`. You only need to build dependencies once. Add the executable to your path if you want to use it anywhere.

## Platform Support
I have tested successfully on Windows, MacOS, and Ubuntu. LuauBundle should support all platforms, but it is mainly limited by the file watcher. If for some reason the file watcher does not work, you can easily remove it by not including it in the make file and removing it's functionality from `main.cpp`.

## Flags
> -f \<file> \
Changes the file name of the main file. The default is main.lua

> -o \<file> \
Changes the output file path. The default is bundle.lua

> -p \
Enables performance mode. Currently this only disables tabs.

> -w \
Enables watch functionality. Watches Lua files in the working directory recursively, excluding the output file.
