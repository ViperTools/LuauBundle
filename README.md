# LuaBundle
LuaBundle is a Lua(u) bundler that uses the require function to allow you to include other lua files and output one bundle file.

## Building
LuaBundle uses [vmake](https://github.com/ViperTools/vmake) for building. Run `vmake -i` to build dependencies, then run `vmake -b` to build, or simply run `vmake -bi`. You only need to build dependencies once.

## Flags
> -f \<file> \
Changes the file name of the main file. The default is main.lua

> -o \<file> \
Changes the output file path. The default is bundle.lua

> -p \
Enables performance mode. This currently only disables tabs.

> -w \
Enabled watch functionality. Watches Lua files in the working directory recursively, excluding the output file.
