// Copyright 2026
#include <gtest/gtest.h>
#include "../include/textgen.h"
#include <stdexcept>
#include <ctime>

// 1. Проверка размера префикса в таблице (все префиксы имеют заданную длину)
TEST(MarkovTest, AllPrefixesHaveCorrectSize) {
    MarkovTextGenerator gen("a b c a b d", 2);
    for (const auto& entry : gen.getTable())
        EXPECT_EQ(entry.first.size(), 2u);
}

// 2. Одна пара префикс-суффикс (два слова)
TEST(MarkovTest, SinglePrefixSuffixPair) {
    MarkovTextGenerator gen("first second third", 2);
    auto& table = gen.getTable();
    prefix p = { "first", "second" };
    ASSERT_TRUE(table.find(p) != table.end());
    EXPECT_EQ(table.at(p).size(), 1u);
    EXPECT_EQ(table.at(p)[0], "third");
}

// 3. Единственный суффикс у префикса (nPref=1)
TEST(MarkovTest, OneSuffixForPrefix) {
    MarkovTextGenerator gen("hello world", 1);
    auto& table = gen.getTable();
    prefix p = { "hello" };
    EXPECT_EQ(table.at(p)[0], "world");
}

// 4. Несколько суффиксов у одного префикса
TEST(MarkovTest, MultipleSuffixes) {
    MarkovTextGenerator gen("a b c a b d", 2);
    prefix p = { "a", "b" };
    ASSERT_TRUE(gen.getTable().find(p) != gen.getTable().end());
    const auto& suffixes = gen.getTable().at(p);
    EXPECT_EQ(suffixes.size(), 2u);
    EXPECT_TRUE(suffixes[0] == "c" || suffixes[1] == "c");
    EXPECT_TRUE(suffixes[0] == "d" || suffixes[1] == "d");
}

// 5. Генерация линейной цепочки (без ветвления)
TEST(MarkovTest, LinearGeneration) {
    MarkovTextGenerator gen("a b c d e", 2);
    std::string result = gen.generate(5);
    // должно быть ровно "a b c d e " (с пробелом в конце)
    EXPECT_EQ(result, "a b c d e ");
}

// 6. Генерация с единственным суффиксом даёт предсказуемую строку
TEST(MarkovTest, SinglePathDeterministic) {
    MarkovTextGenerator gen("x y z", 2);
    std::string res = gen.generate(3);
    EXPECT_EQ(res, "x y z ");
}

// 7. Генерация с ветвлением: оба варианта должны появляться при многократном запуске
TEST(MarkovTest, BranchingBothOptionsAppear) {
    MarkovTextGenerator gen("a b x a b y", 2);
    bool foundX = false, foundY = false;
    for (int i = 0; i < 50; ++i) {
        std::string res = gen.generate(3);
        if (res == "a b x ") foundX = true;
        if (res == "a b y ") foundY = true;
    }
    EXPECT_TRUE(foundX && foundY);
}

// 8. Генерация не выходит за максимальное число слов
TEST(MarkovTest, GenerationRespectsMaxWords) {
    MarkovTextGenerator gen("one two three four five six", 2);
    std::string res = gen.generate(4);
    std::istringstream iss(res);
    int count = 0;
    std::string word;
    while (iss >> word) ++count;
    EXPECT_LE(count, 4);
}

// 9. Обработка пустого текста – выбрасывается исключение
TEST(MarkovTest, EmptyTextThrows) {
    EXPECT_THROW(MarkovTextGenerator("", 2), std::invalid_argument);
}

// 10. Текст короче префикса – исключение
TEST(MarkovTest, TooShortTextThrows) {
    EXPECT_THROW(MarkovTextGenerator("a b", 3), std::invalid_argument);
}

// 11. Префикс из трёх слов строится корректно
TEST(MarkovTest, ThreeWordPrefix) {
    MarkovTextGenerator gen("a b c d e f", 3);
    const auto& table = gen.getTable();
    prefix p = { "a", "b", "c" };
    EXPECT_TRUE(table.find(p) != table.end());
    EXPECT_EQ(table.at(p)[0], "d");
}

// 12. Генерация при nPref=1 работает
TEST(MarkovTest, OneWordPrefix) {
    MarkovTextGenerator gen("cat dog cat bird", 1);
    std::string res = gen.generate(3);
    // первые два слова должны быть cat dog или cat bird
    bool valid = (res == "cat dog " || res == "cat bird ");
    EXPECT_TRUE(valid);
}

// 13. Таблица не содержит префиксов с неправильным числом слов
TEST(MarkovTest, TablePrefixSizeUniform) {
    MarkovTextGenerator gen("a b c d e f g", 2);
    for (auto& entry : gen.getTable())
        EXPECT_EQ(entry.first.size(), 2u);
}

// 14. Генерация останавливается, если префикс не найден (тупик)
TEST(MarkovTest, StopOnMissingPrefix) {
    MarkovTextGenerator gen("a b c", 2);
    std::string res = gen.generate(10);
    // слов должно быть ровно 3 (a b c)
    std::istringstream iss(res);
    int count = 0;
    std::string word;
    while (iss >> word) ++count;
    EXPECT_EQ(count, 3);
}

// 15. Начальный префикс сохраняется при обучении
TEST(MarkovTest, FirstPrefixCorrectlySaved) {
    MarkovTextGenerator gen("first second third", 2);
    prefix expected = { "first", "second" };
    EXPECT_EQ(gen.getFirstPrefix(), expected);
}

// main для Google Test уже внутри библиотеки
