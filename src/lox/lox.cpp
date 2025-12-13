#include <string>
#include <fstream>
#include <iostream>
#include <string_view>

std::string read_file(std::string_view path) {
    std::ifstream file{path.data(), std::ios::in | std::ios::binary | std::ios::ate};

    if (!file) {
        std::cerr << "Failed to open file "
            << path
            << ": "
            << strerror(errno)
            << "\n";
        std::exit(74);
    }

    std::string contents;
    contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(contents.data(), contents.size());

    return contents;
}

void run(std::string_view source) {
    
}

void run_file(std::string_view path) {
    std::string contents = read_file(path);
    run
}
