// Copyright 2026 Kashin Georgii

#include "textgen.h"

#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

MarkovTextGenerator::MarkovTextGenerator(
    const std::string& text, int nPref) {
    std::istringstream iss(text);
    std::vector<std::string> words;
    std::string w;
    while (iss >> w) {
        words.push_back(w);
    }

    if (nPref >= static_cast<int>(words.size())) {
        throw std::invalid_argument("nPref must be < number of words");
    }

    for (int i = 0; i < nPref; ++i) {
        firstPrefix.push_back(words[i]);
    }

    prefix curr;
    for (int i = 0; i < nPref; ++i) {
        curr.push_back(words[i]);
    }

    for (size_t i = nPref; i < words.size(); ++i) {
        table[curr].push_back(words[i]);
        curr.pop_front();
        curr.push_back(words[i]);
    }
}

MarkovTextGenerator::MarkovTextGenerator(
    std::map<prefix, suffix> table) {
    this->table = table;
    if (!table.empty()) {
        firstPrefix = table.begin()->first;
    }
}

std::string MarkovTextGenerator::generate(int maxLen) {
    if (table.empty()) {
        return "";
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::string result;
    prefix curr = firstPrefix;
    for (const auto& w : curr) {
        result += w + " ";
    }

    int wordCount = static_cast<int>(curr.size());

    while (wordCount < maxLen) {
        auto it = table.find(curr);
        if (it == table.end() || it->second.empty()) {
            break;
        }

        const auto& suffixes = it->second;
        int idx = std::rand() % suffixes.size();
        std::string next = suffixes[idx];

        result += next + " ";
        curr.pop_front();
        curr.push_back(next);
        ++wordCount;
    }

    return result;
}
