#include "gtest.h"
#include "hierarchicalList.h"
#include <string>
#include<iostream>

TEST(SampleTest, Basic) {
    EXPECT_EQ(1, 1);
}

// Тестирование вставки следующего элемента
TEST(HierarchicalListTest, InsertNext) {
    HierarchicalList list;
    auto it = list.begin();
    auto it2 = list.insert_next(it);
    EXPECT_NE(it2, list.end());
    EXPECT_EQ(*it2, ""); // по умолчанию пустая строка
}

// Тест обхода списка (прямой проход)
/*TEST(HierarchicalListTest, Traversal) {
    HierarchicalList list;

    // Вставляем первый элемент
    auto it = list.begin();
    // Вставляем следующий
    auto it2 = list.insert_next(it);
    *it2 = "First";

    // Вставляем вниз
    auto downIt = list.insert_down(it);
    *downIt = "Down";

    // Обход
    std::vector<std::string> values;
    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        values.push_back(*iter);
        // Можно дополнительно проверить уровни или структуру
    }

    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], "First");
}*/



// Тест операторов сравнения итераторов
TEST(HierarchicalListTest, IteratorComparison) {
    HierarchicalList list;
    auto it1 = list.begin();
    auto it2 = list.begin();
    EXPECT_TRUE(it1 == it2);
    auto n1 = list.insert_next(it1);
    EXPECT_FALSE(it1 == n1);
}

// Тест печати
TEST(HierarchicalListTest, PrintOutput) {
    HierarchicalList list;
    auto it = list.begin();
    auto n1 = list.insert_next(it);
    *n1 = "Item1";
    auto downIt = list.insert_down(n1);
    *downIt = "DownItem";
    stringstream ss;
    ss << list;
    string output = ss.str();
    // Проверяем наличие ожидаемых строк
    EXPECT_NE(output.find("Item1"), string::npos);
}