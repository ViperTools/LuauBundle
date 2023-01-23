#include <LuauBundle.hpp>
#include <FileWatch/FileWatch.hpp>
#include <iostream>
#include <unistd.h>

std::string inputFile = "main.lua", outputFile = "bundle.lua";

void Bundle(const std::filesystem::path& path, const LuauBundle::BundleOptions& options) {
    std::ofstream file(outputFile, std::ios::trunc);

    LuauBundle::Bundle bundle(path, options);
    std::string source = LuauBundle::moduleSource + bundle.source;
    LuauBundle::Util::Replace(source, "lineMap = {}", "lineMap = " + bundle.BuildLineMap());

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

    if (!std::filesystem::exists(inputFile)) {
        std::cerr << "Input file \"" << inputFile << "\" does not exist\n";
        return 1;
    }

    LuauBundle::BundleOptions options;
    options.Tab = !performance;

    std::filesystem::path path = std::filesystem::current_path().append(inputFile);
    Bundle(path, options);

    if (watch) {
        std::filesystem::path outputPath = std::filesystem::path(outputFile);

        filewatch::FileWatch<std::string> watch(
            "./",
            std::regex(".*\\.lua"),
            [&path, &options, &outputPath](const std::string _path, const filewatch::Event changeType) {
                if (!std::filesystem::equivalent(_path, outputPath) && changeType == filewatch::Event::modified) {
                    Bundle(path, options);
                }
            }
        );

        std::cout << "[LuauBundle] Watching for file changes" << std::endl;

        std::cin.get();
    }

    return 0;
}