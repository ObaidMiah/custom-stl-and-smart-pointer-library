#include <gtest/gtest.h>
#include "../include/stack.h"
#include <string>

class StackTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(StackTest, DefaultIsEmpty) {
    Stack<int> s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
}

TEST_F(StackTest, PushTopSize) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_EQ(s.size(), 3u);
    EXPECT_EQ(s.top(), 3);         // last pushed is on top
    EXPECT_FALSE(s.empty());
}

TEST_F(StackTest, LifoOrder) {
    Stack<int> s;
    s.push(1); s.push(2); s.push(3);
    EXPECT_EQ(s.top(), 3); s.pop();
    EXPECT_EQ(s.top(), 2); s.pop();
    EXPECT_EQ(s.top(), 1); s.pop();
    EXPECT_TRUE(s.empty());
}

TEST_F(StackTest, TopIsMutable) {
    Stack<int> s;
    s.push(10);
    s.top() = 99;
    EXPECT_EQ(s.top(), 99);
}

TEST_F(StackTest, PushPopInterleaved) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.pop();                       // removes 2
    s.push(3);
    EXPECT_EQ(s.top(), 3);
    s.pop();
    EXPECT_EQ(s.top(), 1);
    EXPECT_EQ(s.size(), 1u);
}

TEST_F(StackTest, WorksWithStrings) {
    Stack<std::string> s;
    s.push("a");
    s.push("b");
    EXPECT_EQ(s.top(), "b");
    s.pop();
    EXPECT_EQ(s.top(), "a");
}
