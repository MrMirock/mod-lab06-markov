// Copyright 2026 Kashin Georgii

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <sstream>

#include "textgen.h"

// 1
TEST(MarkovTest, AllPrefixesHaveCorrectSize) {
    MarkovTextGenerator gen("a b c a b d", 2);
    for (const auto& entry : gen.getTable()) {
        EXPECT_EQ(entry.first.size(), 2u);
    }
}

// 2
TEST(MarkovTest, SinglePrefixSuffixPair) {
    MarkovTextGenerator gen("first second third", 2);
    auto& table = gen.getTable();
    prefix p = { "first", "second" };
    ASSERT_TRUE(table.find(p) != table.end());
    EXPECT_EQ(table.at(p).size(), 1u);
    EXPECT_EQ(table.at(p)[0], "third");
}

// 3
TEST(MarkovTest, OneSuffixForPrefix) {
    MarkovTextGenerator gen("hello world", 1);
    auto& table = gen.getTable();
    prefix p = { "hello" };
    EXPECT_EQ(table.at(p)[0], "world");
}

// 4
TEST(MarkovTest, MultipleSuffixes) {
    MarkovTextGenerator gen("a b c a b d", 2);
    prefix p = { "a", "b" };
    ASSERT_TRUE(gen.getTable().find(p) != gen.getTable().end());
    const auto& suffixes = gen.getTable().at(p);
    EXPECT_EQ(suffixes.size(), 2u);
    EXPECT_TRUE(suffixes[0] == "c" || suffixes[1] == "c");
    EXPECT_TRUE(suffixes[0] == "d" || suffixes[1] == "d");
}

// 5
TEST(MarkovTest, LinearGeneration) {
    MarkovTextGenerator gen("a b c d e", 2);
    std::string result = gen.generate(5);
    EXPECT_EQ(result, "a b c d e ");
}

// 6
TEST(MarkovTest, SinglePathDeterministic) {
    MarkovTextGenerator gen("x y z", 2);
    std::string res = gen.generate(3);
    EXPECT_EQ(res, "x y z ");
}

// 7
TEST(MarkovTest, BranchingBothOptionsAppear) {
  MarkovTextGenerator gen("a b x a b y", 2);
  std::string res = gen.generate(3);
  bool valid = (res == "a b x " || res == "a b y ");
  EXPECT_TRUE(valid);
}

// 8
TEST(MarkovTest, GenerationRespectsMaxWords) {
    MarkovTextGenerator gen("one two three four five six", 2);
    std::string res = gen.generate(4);
    std::istringstream iss(res);
    int count = 0;
    std::string word;
    while (iss >> word) ++count;
    EXPECT_LE(count, 4);
}

// 9
TEST(MarkovTest, EmptyTextThrows) {
    EXPECT_THROW(MarkovTextGenerator("", 2), std::invalid_argument);
}

// 10
TEST(MarkovTest, TooShortTextThrows) {
    EXPECT_THROW(MarkovTextGenerator("a b", 3), std::invalid_argument);
}

// 11
TEST(MarkovTest, ThreeWordPrefix) {
    MarkovTextGenerator gen("a b c d e f", 3);
    const auto& table = gen.getTable();
    prefix p = { "a", "b", "c" };
    EXPECT_TRUE(table.find(p) != table.end());
    EXPECT_EQ(table.at(p)[0], "d");
}

// 12
TEST(MarkovTest, OneWordPrefix) {
    MarkovTextGenerator gen("cat dog cat bird", 1);
    std::string res = gen.generate(3);
    bool valid = (res == "cat dog " || res == "cat bird ");
    EXPECT_TRUE(valid);
}

// 13
TEST(MarkovTest, TablePrefixSizeUniform) {
    MarkovTextGenerator gen("a b c d e f g", 2);
    for (const auto& entry : gen.getTable()) {
        EXPECT_EQ(entry.first.size(), 2u);
    }
}

// 14
TEST(MarkovTest, StopOnMissingPrefix) {
    MarkovTextGenerator gen("a b c", 2);
    std::string res = gen.generate(10);
    std::istringstream iss(res);
    int count = 0;
    std::string word;
    while (iss >> word) {
        ++count;
    }
    EXPECT_EQ(count, 3);
}

// 15
TEST(MarkovTest, FirstPrefixCorrectlySaved) {
    MarkovTextGenerator gen("first second third", 2);
    prefix expected = { "first", "second" };
    EXPECT_EQ(gen.getFirstPrefix(), expected);
}
