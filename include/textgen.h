#pragma once
#include <string>
#include <deque>
#include <map>
#include <vector>

typedef std::deque<std::string> prefix;
typedef std::vector<std::string> suffix;

class MarkovTextGenerator {
public:
    MarkovTextGenerator(const std::string& text, int nPref = 2);

    std::string generate(int maxLen);
    const std::map<prefix, suffix>& getTable() const { return table; }
    const prefix& getFirstPrefix() const { return firstPrefix; }

private:
    std::map<prefix, suffix> table;
    prefix firstPrefix;
};