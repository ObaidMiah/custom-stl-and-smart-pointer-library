#include <gtest/gtest.h>
#include "../include/shared_ptr.h"
#include <string>
#include <utility>

class SharedPtrTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Counts live instances so we can prove objects are freed at the right time.
struct Tracked {
    static int alive;
    int value;
    explicit Tracked(int v = 0) : value(v) { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

// --- Basics ---------------------------------------------------------------
TEST_F(SharedPtrTest, DefaultIsEmpty) {
    SharedPtr<int> p;
    EXPECT_EQ(p.get(), nullptr);
    EXPECT_FALSE(static_cast<bool>(p));
    EXPECT_EQ(p.use_count(), 0u);
}

TEST_F(SharedPtrTest, OwningConstructorHasCountOne) {
    SharedPtr<int> p(new int(42));
    EXPECT_TRUE(static_cast<bool>(p));
    EXPECT_EQ(*p, 42);
    EXPECT_EQ(p.use_count(), 1u);
}

TEST_F(SharedPtrTest, ArrowOperator) {
    SharedPtr<std::string> p(new std::string("hello"));
    EXPECT_EQ(p->size(), 5u);
}

// --- Reference counting ---------------------------------------------------
TEST_F(SharedPtrTest, CopyIncrementsCountSharesObject) {
    EXPECT_EQ(Tracked::alive, 0);
    SharedPtr<Tracked> a(new Tracked(1));
    {
        SharedPtr<Tracked> b = a;          // copy
        EXPECT_EQ(a.use_count(), 2u);
        EXPECT_EQ(b.use_count(), 2u);
        EXPECT_EQ(a.get(), b.get());       // same object
        EXPECT_EQ(Tracked::alive, 1);      // still one object
    }
    EXPECT_EQ(a.use_count(), 1u);          // b gone, count dropped
    EXPECT_EQ(Tracked::alive, 1);
}

TEST_F(SharedPtrTest, ObjectFreedWhenLastOwnerDies) {
    EXPECT_EQ(Tracked::alive, 0);
    {
        SharedPtr<Tracked> a(new Tracked(1));
        SharedPtr<Tracked> b = a;
        EXPECT_EQ(Tracked::alive, 1);
    }
    EXPECT_EQ(Tracked::alive, 0);          // freed only after BOTH gone
}

TEST_F(SharedPtrTest, CopyAssignmentReleasesOldSharesNew) {
    EXPECT_EQ(Tracked::alive, 0);
    SharedPtr<Tracked> a(new Tracked(1));
    SharedPtr<Tracked> b(new Tracked(2));
    EXPECT_EQ(Tracked::alive, 2);

    b = a;                                 // b's old object freed; now shares a's
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(a.use_count(), 2u);
    EXPECT_EQ(b->value, 1);
}

TEST_F(SharedPtrTest, SelfAssignmentIsSafe) {
    SharedPtr<Tracked> a(new Tracked(5));
    SharedPtr<Tracked>& alias = a;
    a = alias;                             // must not free its own object
    EXPECT_EQ(a.use_count(), 1u);
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(a->value, 5);
}

// --- Move semantics -------------------------------------------------------
TEST_F(SharedPtrTest, MoveTransfersWithoutBumpingCount) {
    SharedPtr<Tracked> a(new Tracked(1));
    EXPECT_EQ(a.use_count(), 1u);
    SharedPtr<Tracked> b(std::move(a));
    EXPECT_EQ(b.use_count(), 1u);          // count unchanged by move
    EXPECT_EQ(a.get(), nullptr);           // source emptied
    EXPECT_EQ(Tracked::alive, 1);
}

TEST_F(SharedPtrTest, ResetReleasesOwnership) {
    EXPECT_EQ(Tracked::alive, 0);
    SharedPtr<Tracked> a(new Tracked(1));
    a.reset();
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(Tracked::alive, 0);
}

// --- WeakPtr --------------------------------------------------------------
TEST_F(SharedPtrTest, WeakDoesNotKeepObjectAlive) {
    EXPECT_EQ(Tracked::alive, 0);
    WeakPtr<Tracked> w;
    {
        SharedPtr<Tracked> s(new Tracked(1));
        w = WeakPtr<Tracked>(s);
        EXPECT_EQ(s.use_count(), 1u);      // weak does NOT raise shared count
        EXPECT_FALSE(w.expired());
        EXPECT_EQ(w.use_count(), 1u);
    }
    // s destroyed → object freed, even though w still observes the block.
    EXPECT_EQ(Tracked::alive, 0);
    EXPECT_TRUE(w.expired());
    EXPECT_EQ(w.use_count(), 0u);
}

TEST_F(SharedPtrTest, LockReturnsOwnerWhenAlive) {
    SharedPtr<Tracked> s(new Tracked(7));
    WeakPtr<Tracked> w(s);

    SharedPtr<Tracked> locked = w.lock();
    EXPECT_TRUE(static_cast<bool>(locked));
    EXPECT_EQ(locked->value, 7);
    EXPECT_EQ(s.use_count(), 2u);          // lock() created a real co-owner
}

TEST_F(SharedPtrTest, LockReturnsEmptyWhenExpired) {
    WeakPtr<Tracked> w;
    {
        SharedPtr<Tracked> s(new Tracked(1));
        w = WeakPtr<Tracked>(s);
    }
    SharedPtr<Tracked> locked = w.lock();
    EXPECT_FALSE(static_cast<bool>(locked));   // object gone → empty
    EXPECT_EQ(locked.get(), nullptr);
}

TEST_F(SharedPtrTest, WeakOutlivingObjectDoesNotLeakBlock) {
    // Object dies first (shared_count 0), block must survive until weak gone,
    // then be freed. ASan verifies no leak / no double free here.
    WeakPtr<Tracked> w;
    {
        SharedPtr<Tracked> s(new Tracked(1));
        w = WeakPtr<Tracked>(s);
    }                                      // object freed, block kept for w
    EXPECT_TRUE(w.expired());
}                                          // w destroyed → block freed

// --- Reference cycle: the reason WeakPtr exists ---------------------------
struct Node {
    static int alive;
    SharedPtr<Node> next;     // strong
    WeakPtr<Node> prev;       // weak — breaks the cycle
    Node() { ++alive; }
    ~Node() { --alive; }
};
int Node::alive = 0;

TEST_F(SharedPtrTest, WeakBreaksReferenceCycle) {
    EXPECT_EQ(Node::alive, 0);
    {
        SharedPtr<Node> a(new Node());
        SharedPtr<Node> b(new Node());
        a->next = b;          // a strongly owns b
        b->prev = WeakPtr<Node>(a);   // b only weakly observes a
        EXPECT_EQ(Node::alive, 2);
    }
    // Because b->prev is weak, no strong cycle exists, so both free cleanly.
    EXPECT_EQ(Node::alive, 0);
}
