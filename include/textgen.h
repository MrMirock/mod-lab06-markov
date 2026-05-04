// Copyright 2026 Kashin Georgii

#pragma once

#include <deque>
#include <map>
#include <string>
#include <vector>

typedef std::deque<std::string> prefix;
typedef std::vector<std::string> suffix;

class MarkovTextGenerator {
 public:
  explicit MarkovTextGenerator(const std::string& text, int nPref = 2);
  explicit MarkovTextGenerator(std::map<prefix, suffix> table);

  std::string generate(int maxLen);

  const std::map<prefix, suffix>& getTable() const { return table; }
  const prefix& getFirstPrefix() const { return firstPrefix; }

 private:
  std::map<prefix, suffix> table;
  prefix firstPrefix;
};
