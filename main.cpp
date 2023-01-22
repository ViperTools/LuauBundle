#include <LuaBundle.hpp>
#include <FileWatch/FileWatch.hpp>
#include <iostream>
#include <unistd.h>

std::string inputFile = "main.lua", outputFile = "bundle.lua";

void Bundle(const std::filesystem::path& path, const LuaBundle::BundleOptions& options) {
    std::ofstream file(outputFile, std::ios::trunc);

    LuaBundle::Bundle bundle(path, options);
    std::string source = LuaBundle::moduleSource + bundle.source;
    LuaBundle::Util::Replace(source, "lineMap = {}", "lineMap = " + bundle.BuildLineMap());

    file << source;

    file.close();
}

int main(int argc, char* argv[]) {
    int opt;
    bool performance = false, watch = false;

    while ((opt = getopt(argc, argv, "f:o:pw")) != -1) {
        switch (opt) {
            case 'f': {
                inputFile = optarg;
                break;
            }
            case 'o': {
                outputFile = optarg;
                break;
            }
            case 'p': {
                performance = true;
                break;
            }
            case 'w': {
                watch = true;
                break;
            }
        }
    }

    LuaBundle::BundleOptions options;
    options.Tab = !performance;

    std::filesystem::path path = std::filesystem::current_path().append(inputFile);
    Bundle(path, options);

    if (watch) {
        filewatch::FileWatch<std::string> watch(
            "./",
            std::regex("^(?!" + outputFile + R"(\.lua$).*\.lua)"),
            [&path, &options](const std::string _path, const filewatch::Event changeType) {
                if (changeType == filewatch::Event::modified) {
                    Bundle(path, options);
                }
            }
        );

        std::cout << "[LuaBundle] Watching for file changes" << std::endl;

        std::cin.get();
    }

    return 0;
}