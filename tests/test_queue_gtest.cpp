#include <gtest/gtest.h>
#include "../include/queue.h"
#include <string>

class QueueTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(QueueTest, DefaultIsEmpty) {
    Queue<int> q;
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0u);
}

TEST_F(QueueTest, PushFrontBackSize) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    EXPECT_EQ(q.size(), 3u);
    EXPECT_EQ(q.front(), 1);       // oldest is at the front
    EXPECT_EQ(q.back(), 3);        // newest is at the back
}

TEST_F(QueueTest, FifoOrder) {
    Queue<int> q;
    q.push(1); q.push(2); q.push(3);
    EXPECT_EQ(q.front(), 1); q.pop();
    EXPECT_EQ(q.front(), 2); q.pop();
    EXPECT_EQ(q.front(), 3); q.pop();
    EXPECT_TRUE(q.empty());
}

TEST_F(QueueTest, FrontIsMutable) {
    Queue<int> q;
    q.push(10);
    q.front() = 99;
    EXPECT_EQ(q.front(), 99);
}

TEST_F(QueueTest, PushPopInterleaved) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.pop();                       // removes 1
    q.push(3);
    EXPECT_EQ(q.front(), 2);
    EXPECT_EQ(q.back(), 3);
    EXPECT_EQ(q.size(), 2u);
}

TEST_F(QueueTest, WorksWithStrings) {
    Queue<std::string> q;
    q.push("a");
    q.push("b");
    EXPECT_EQ(q.front(), "a");
    EXPECT_EQ(q.back(), "b");
    q.pop();
    EXPECT_EQ(q.front(), "b");
}
