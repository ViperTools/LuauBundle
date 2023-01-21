# LuaBundle
LuaBundle is a Lua(u) bundler that uses the require function to allow you to include other lua files and output one bundle file.

## Flags
> -f \<file> \
Changes the file name of the main file. The default is main.lua

> -o \<file> \
Changes the output file path. The default is bundle.lua

> -p \
Enables performance mode. This currently only disables tabs.

> -w \
Enabled watch functionality. Watches Lua files in the working directory recursively, excluding the output file.