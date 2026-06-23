#include <gtest/gtest.h>
#include "../include/map.h"
#include <string>

class MapTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// --- Basics ---------------------------------------------------------------
TEST_F(MapTest, DefaultIsEmpty) {
    Map<int, int> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_FALSE(m.contains(1));
}

TEST_F(MapTest, InsertViaSubscriptAndRead) {
    Map<int, std::string> m;
    m[1] = "one";
    m[2] = "two";
    EXPECT_EQ(m.size(), 2u);
    EXPECT_EQ(m[1], "one");
    EXPECT_EQ(m[2], "two");
    EXPECT_TRUE(m.contains(1));
    EXPECT_TRUE(m.contains(2));
}

TEST_F(MapTest, SubscriptOnMissingKeyInsertsDefault) {
    Map<int, int> m;
    int v = m[5];                  // default-constructed int = 0
    EXPECT_EQ(v, 0);
    EXPECT_EQ(m.size(), 1u);       // accessing a missing key inserts it
    EXPECT_TRUE(m.contains(5));
}

TEST_F(MapTest, SubscriptDoesNotOverwriteExisting) {
    Map<int, int> m;
    m[1] = 100;
    (void)m[1];                    // touching an existing key must not reset it
    EXPECT_EQ(m[1], 100);
    EXPECT_EQ(m.size(), 1u);
}

TEST_F(MapTest, SubscriptUpdatesValue) {
    Map<int, int> m;
    m[1] = 10;
    m[1] = 20;                     // overwrite via assignment
    EXPECT_EQ(m[1], 20);
    EXPECT_EQ(m.size(), 1u);       // still one key
}

// --- at -------------------------------------------------------------------
TEST_F(MapTest, AtReturnsValueOrThrows) {
    Map<int, int> m;
    m[7] = 70;
    EXPECT_EQ(m.at(7), 70);
    EXPECT_THROW(m.at(99), std::out_of_range);
    EXPECT_EQ(m.size(), 1u);       // at() must NOT insert on miss
}

TEST_F(MapTest, AtAllowsMutation) {
    Map<int, int> m;
    m[1] = 1;
    m.at(1) = 42;
    EXPECT_EQ(m[1], 42);
}

// --- erase ----------------------------------------------------------------
TEST_F(MapTest, EraseLeaf) {
    Map<int, int> m;
    m[2] = 2; m[1] = 1; m[3] = 3;  // 1 and 3 are leaves
    EXPECT_TRUE(m.erase(1));
    EXPECT_FALSE(m.contains(1));
    EXPECT_EQ(m.size(), 2u);
    EXPECT_TRUE(m.contains(2));
    EXPECT_TRUE(m.contains(3));
}

TEST_F(MapTest, EraseNodeWithOneChild) {
    Map<int, int> m;
    m[2] = 2; m[1] = 1; m[3] = 3; m[4] = 4;   // 3 has only a right child (4)
    EXPECT_TRUE(m.erase(3));
    EXPECT_FALSE(m.contains(3));
    EXPECT_TRUE(m.contains(4));                // child survived
    EXPECT_EQ(m.size(), 3u);
}

TEST_F(MapTest, EraseNodeWithTwoChildren) {
    Map<int, int> m;
    // Root 5 has two subtrees; erasing it exercises the successor path.
    for (int k : {5, 3, 8, 2, 4, 7, 9}) m[k] = k * 10;
    EXPECT_EQ(m.size(), 7u);
    EXPECT_TRUE(m.erase(5));
    EXPECT_FALSE(m.contains(5));
    EXPECT_EQ(m.size(), 6u);
    // Everything else still present and intact.
    for (int k : {2, 3, 4, 7, 8, 9}) {
        EXPECT_TRUE(m.contains(k));
        EXPECT_EQ(m.at(k), k * 10);
    }
}

TEST_F(MapTest, EraseMissingKeyReturnsFalse) {
    Map<int, int> m;
    m[1] = 1;
    EXPECT_FALSE(m.erase(99));
    EXPECT_EQ(m.size(), 1u);
}

TEST_F(MapTest, EraseRootDownToEmpty) {
    Map<int, int> m;
    m[1] = 1;
    EXPECT_TRUE(m.erase(1));
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_FALSE(m.contains(1));
}

// --- clear ----------------------------------------------------------------
TEST_F(MapTest, ClearEmptiesMap) {
    Map<int, int> m;
    for (int k : {5, 3, 8, 1, 4}) m[k] = k;
    m.clear();
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_FALSE(m.contains(5));
    m[10] = 10;                    // usable again after clear
    EXPECT_EQ(m[10], 10);
}

// --- String keys ----------------------------------------------------------
TEST_F(MapTest, StringKeys) {
    Map<std::string, int> m;
    m["apple"] = 1;
    m["banana"] = 2;
    EXPECT_EQ(m.at("apple"), 1);
    EXPECT_TRUE(m.contains("banana"));
    EXPECT_TRUE(m.erase("apple"));
    EXPECT_FALSE(m.contains("apple"));
}

// --- A larger sequence of mixed operations --------------------------------
TEST_F(MapTest, StressInsertEraseConsistency) {
    Map<int, int> m;
    for (int i = 0; i < 50; ++i) m[i] = i * i;
    EXPECT_EQ(m.size(), 50u);
    for (int i = 0; i < 50; i += 2) EXPECT_TRUE(m.erase(i));   // remove evens
    EXPECT_EQ(m.size(), 25u);
    for (int i = 0; i < 50; ++i) {
        if (i % 2 == 0) EXPECT_FALSE(m.contains(i));
        else { EXPECT_TRUE(m.contains(i)); EXPECT_EQ(m.at(i), i * i); }
    }
}
