#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/textgen.h"  // путь к заголовку

int main() {
    std::ifstream file("C:\Users\Tolya\source\repos\mod-lab06-markov\input\input.txt");
    if (!file) {
        std::cerr << "Error opening input.txt\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    MarkovTextGenerator gen(text, 2);
    std::string result = gen.generate(1500);

    // Сохраняем в result/gen.txt
    std::ofstream out("C:\Users\Tolya\source\repos\mod-lab06-markov\result\gen.txt");
    if (!out) {
        std::cerr << "Error creating gen.txt\n";
        return 1;
    }
    out << result;
    out.close();

    std::cout << "Text saved to result/gen.txt\n";
    return 0;
}