#include "gtest.h"
#include "hierarchicalList.h"
#include <string>
#include<iostream>

TEST(SampleTest, Basic) {
    EXPECT_EQ(1, 1);
}

// ������������ ������� ���������� ��������
TEST(HierarchicalListTest, InsertNext) {
    HierarchicalList list;
    auto it = list.begin();
    auto it2 = list.insert_next(it);
    EXPECT_NE(it2, list.end());
    EXPECT_EQ(*it2, ""); // �� ��������� ������ ������
}

// ���� ������ ������ (������ ������)
/*TEST(HierarchicalListTest, Traversal) {
    HierarchicalList list;

    // ��������� ������ �������
    auto it = list.begin();
    // ��������� ���������
    auto it2 = list.insert_next(it);
    *it2 = "First";

    // ��������� ����
    auto downIt = list.insert_down(it);
    *downIt = "Down";

    // �����
    std::vector<std::string> values;
    for (auto iter = list.begin(); iter != list.end(); ++iter) {
        values.push_back(*iter);
        // ����� ������������� ��������� ������ ��� ���������
    }

    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], "First");
}*/



// ���� ���������� ��������� ����������
TEST(HierarchicalListTest, IteratorComparison) {
    HierarchicalList list;
    auto it1 = list.begin();
    auto it2 = list.begin();
    EXPECT_TRUE(it1 == it2);
    auto n1 = list.insert_next(it1);
    EXPECT_FALSE(it1 == n1);
}

// ���� ������
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
    // ��������� ������� ��������� �����
    EXPECT_NE(output.find("Item1"), string::npos);
}