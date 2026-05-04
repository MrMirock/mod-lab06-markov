// Copyright 2026 Kashin Georgii

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "textgen.h"

int main() {
    std::ifstream file("input.txt");
    if (!file) {
        std::cerr << "Error opening input.txt\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    MarkovTextGenerator gen(text, 2);
    std::string result = gen.generate(1500);

    std::ofstream out("output.txt");
    if (!out) {
        std::cerr << "Error creating output.txt\n";
        return 1;
    }
    out << result;
    out.close();

    std::cout << "Text saved to output.txt\n";
    return 0;
}
