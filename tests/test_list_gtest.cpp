#include <gtest/gtest.h>
#include "../include/list.h"
#include <string>
#include <vector>

class ListTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Helper: collect a list's contents front-to-back into a vector for comparison.
template<typename T>
static std::vector<T> to_vec(List<T>& l) {
    std::vector<T> out;
    for (auto it = l.begin(); it != l.end(); ++it) out.push_back(*it);
    return out;
}

// --- Construction --------------------------------------------------------
TEST_F(ListTest, DefaultConstructorIsEmpty) {
    List<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0u);
    EXPECT_EQ(l.begin(), l.end());
}

// --- push_back / push_front ----------------------------------------------
TEST_F(ListTest, PushBackAppends) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    EXPECT_EQ(l.size(), 3u);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
    EXPECT_EQ(to_vec(l), (std::vector<int>{1, 2, 3}));
}

TEST_F(ListTest, PushFrontPrepends) {
    List<int> l;
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    EXPECT_EQ(l.size(), 3u);
    EXPECT_EQ(l.front(), 3);
    EXPECT_EQ(l.back(), 1);
    EXPECT_EQ(to_vec(l), (std::vector<int>{3, 2, 1}));
}

TEST_F(ListTest, MixedPushKeepsBothLinks) {
    List<int> l;
    l.push_back(2);
    l.push_front(1);
    l.push_back(3);   // 1 2 3
    EXPECT_EQ(to_vec(l), (std::vector<int>{1, 2, 3}));

    // Walk forward to the last element, then backward to prove prev links
    // are wired too. (end() is nullptr here, so we don't decrement it.)
    auto it = l.begin();
    auto last = it;
    for (; it != l.end(); ++it) last = it;
    EXPECT_EQ(*last, 3);
    --last; EXPECT_EQ(*last, 2);
    --last; EXPECT_EQ(*last, 1);
}

// --- pop_back / pop_front ------------------------------------------------
TEST_F(ListTest, PopBackRemovesLast) {
    List<int> l;
    l.push_back(1); l.push_back(2); l.push_back(3);
    l.pop_back();
    EXPECT_EQ(l.size(), 2u);
    EXPECT_EQ(l.back(), 2);
    EXPECT_EQ(to_vec(l), (std::vector<int>{1, 2}));
}

TEST_F(ListTest, PopFrontRemovesFirst) {
    List<int> l;
    l.push_back(1); l.push_back(2); l.push_back(3);
    l.pop_front();
    EXPECT_EQ(l.size(), 2u);
    EXPECT_EQ(l.front(), 2);
    EXPECT_EQ(to_vec(l), (std::vector<int>{2, 3}));
}

TEST_F(ListTest, PopToEmptyThenReuse) {
    List<int> l;
    l.push_back(42);
    l.pop_back();              // single-element edge case
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0u);
    EXPECT_EQ(l.begin(), l.end());

    l.push_front(7);           // must still work after emptying
    EXPECT_EQ(l.front(), 7);
    EXPECT_EQ(l.back(), 7);
    EXPECT_EQ(l.size(), 1u);
}

TEST_F(ListTest, PopOnEmptyIsNoOp) {
    List<int> l;
    l.pop_back();
    l.pop_front();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0u);
}

// --- front / back error handling -----------------------------------------
TEST_F(ListTest, FrontBackThrowWhenEmpty) {
    List<int> l;
    EXPECT_THROW(l.front(), std::runtime_error);
    EXPECT_THROW(l.back(),  std::runtime_error);
}

// --- Iterators -----------------------------------------------------------
TEST_F(ListTest, PostIncrementReturnsOldPosition) {
    List<int> l;
    l.push_back(10); l.push_back(20);
    auto it = l.begin();
    auto old = it++;
    EXPECT_EQ(*old, 10);   // post-increment yields the previous element
    EXPECT_EQ(*it, 20);
}

TEST_F(ListTest, RangeBasedForWorks) {
    List<int> l;
    for (int i = 1; i <= 4; ++i) l.push_back(i);
    int sum = 0;
    for (int x : l) sum += x;
    EXPECT_EQ(sum, 10);
}

// --- Copy semantics (deep copy) ------------------------------------------
TEST_F(ListTest, CopyConstructorIsDeep) {
    List<int> a;
    a.push_back(1); a.push_back(2); a.push_back(3);

    List<int> b(a);
    EXPECT_EQ(to_vec(b), (std::vector<int>{1, 2, 3}));

    a.push_back(99);                 // mutate original
    EXPECT_EQ(b.size(), 3u);         // copy unaffected
    EXPECT_EQ(to_vec(b), (std::vector<int>{1, 2, 3}));
}

TEST_F(ListTest, CopyAssignmentIsDeepAndSelfSafe) {
    List<int> a;
    a.push_back(1); a.push_back(2);

    List<int> b;
    b.push_back(99);
    b = a;
    EXPECT_EQ(to_vec(b), (std::vector<int>{1, 2}));

    List<int>& alias = b;
    b = alias;                       // self-assignment must not corrupt
    EXPECT_EQ(to_vec(b), (std::vector<int>{1, 2}));
}

// --- Move semantics ------------------------------------------------------
TEST_F(ListTest, MoveConstructorTransfers) {
    List<int> a;
    a.push_back(1); a.push_back(2); a.push_back(3);

    List<int> b(std::move(a));
    EXPECT_EQ(to_vec(b), (std::vector<int>{1, 2, 3}));
    EXPECT_TRUE(a.empty());          // source left empty
    EXPECT_EQ(a.size(), 0u);
}

TEST_F(ListTest, MoveAssignmentTransfers) {
    List<int> a;
    a.push_back(5); a.push_back(6);

    List<int> b;
    b.push_back(99);
    b = std::move(a);
    EXPECT_EQ(to_vec(b), (std::vector<int>{5, 6}));
    EXPECT_TRUE(a.empty());
}

// --- clear ---------------------------------------------------------------
TEST_F(ListTest, ClearEmptiesList) {
    List<int> l;
    l.push_back(1); l.push_back(2); l.push_back(3);
    l.clear();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0u);
    EXPECT_EQ(l.begin(), l.end());
}

// --- insert / erase (optional features) ----------------------------------
TEST_F(ListTest, InsertAtVariousPositions) {
    List<int> l;
    l.push_back(1); l.push_back(3);
    auto it = l.begin(); ++it;            // points at 3
    l.insert(it, 2);                      // 1 2 3
    EXPECT_EQ(to_vec(l), (std::vector<int>{1, 2, 3}));

    l.insert(l.begin(), 0);               // prepend -> 0 1 2 3
    l.insert(l.end(), 4);                 // append  -> 0 1 2 3 4
    EXPECT_EQ(to_vec(l), (std::vector<int>{0, 1, 2, 3, 4}));
    EXPECT_EQ(l.size(), 5u);
}

TEST_F(ListTest, EraseRemovesAndReturnsNext) {
    List<int> l;
    for (int i = 0; i < 4; ++i) l.push_back(i);   // 0 1 2 3
    auto it = l.begin(); ++it;                    // points at 1
    auto next = l.erase(it);                      // remove 1 -> 0 2 3
    EXPECT_EQ(*next, 2);
    EXPECT_EQ(to_vec(l), (std::vector<int>{0, 2, 3}));

    l.erase(l.begin());                           // erase head -> 2 3
    EXPECT_EQ(l.front(), 2);
    EXPECT_EQ(to_vec(l), (std::vector<int>{2, 3}));
}

TEST_F(ListTest, WorksWithStrings) {
    List<std::string> l;
    l.push_back("a");
    l.push_front("b");
    EXPECT_EQ(l.front(), "b");
    EXPECT_EQ(l.back(), "a");
    EXPECT_EQ(l.size(), 2u);
}
