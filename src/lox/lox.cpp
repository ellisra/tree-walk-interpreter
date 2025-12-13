#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>
#include "error.h"
#include "scanner.h"

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
    Scanner scanner{source};
    std::vector<Token> tokens = scanner.scan_tokens();

    for (const Token &token : tokens) {
        std::cout << token.to_string() << "\n";
    }
}

void run_file(std::string_view path) {
    std::string contents = read_file(path);
    run(contents);

    if (had_error) std::exit(65);
}

void run_prompt() {
    for (;;) {
        std::cout << "> ";
        std::string line;

        if (!std::getline(std::cin, line)) break;
        run(line);
        had_error = false;
    }
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cout << "Usage: lox [script]\n";
        std::exit(64);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }
}
