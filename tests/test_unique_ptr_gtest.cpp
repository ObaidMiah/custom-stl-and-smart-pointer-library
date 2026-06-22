#include <gtest/gtest.h>
#include "../include/unique_ptr.h"
#include <string>
#include <utility>

class UniquePtrTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Counts live instances so we can prove the destructor actually runs.
struct Tracked {
    static int alive;
    int value;
    explicit Tracked(int v = 0) : value(v) { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

// --- Construction / basic access -----------------------------------------
TEST_F(UniquePtrTest, DefaultIsEmpty) {
    UniquePtr<int> p;
    EXPECT_EQ(p.get(), nullptr);
    EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(UniquePtrTest, OwningConstructorStoresPointer) {
    int* raw = new int(42);
    UniquePtr<int> p(raw);
    EXPECT_EQ(p.get(), raw);
    EXPECT_TRUE(static_cast<bool>(p));
    EXPECT_EQ(*p, 42);
}

TEST_F(UniquePtrTest, ArrowOperatorReachesMembers) {
    UniquePtr<std::string> p(new std::string("hello"));
    EXPECT_EQ(p->size(), 5u);
    EXPECT_EQ(*p, "hello");
}

// --- Destruction frees the object ----------------------------------------
TEST_F(UniquePtrTest, DestructorDeletesOwnedObject) {
    EXPECT_EQ(Tracked::alive, 0);
    {
        UniquePtr<Tracked> p(new Tracked(7));
        EXPECT_EQ(Tracked::alive, 1);
    }
    EXPECT_EQ(Tracked::alive, 0);   // destructor ran on scope exit
}

TEST_F(UniquePtrTest, EmptyDestructorIsSafe) {
    { UniquePtr<int> p; }           // deleting nullptr must not crash
    SUCCEED();
}

// --- Move semantics (copying is deleted, so only moves) ------------------
TEST_F(UniquePtrTest, MoveConstructorTransfersOwnership) {
    EXPECT_EQ(Tracked::alive, 0);
    UniquePtr<Tracked> a(new Tracked(1));
    Tracked* raw = a.get();

    UniquePtr<Tracked> b(std::move(a));
    EXPECT_EQ(b.get(), raw);        // b now owns it
    EXPECT_EQ(a.get(), nullptr);    // a was emptied
    EXPECT_EQ(Tracked::alive, 1);   // still exactly one object
}

TEST_F(UniquePtrTest, MoveAssignmentTransfersAndFreesOld) {
    EXPECT_EQ(Tracked::alive, 0);
    UniquePtr<Tracked> a(new Tracked(1));
    UniquePtr<Tracked> b(new Tracked(2));
    EXPECT_EQ(Tracked::alive, 2);

    b = std::move(a);               // b's old object freed, a's transferred
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(b->value, 1);
    EXPECT_EQ(a.get(), nullptr);
}

TEST_F(UniquePtrTest, MoveAssignFromEmpty) {
    UniquePtr<int> a;
    UniquePtr<int> b(new int(5));
    b = std::move(a);               // assigning an empty one frees b's object
    EXPECT_EQ(b.get(), nullptr);
}

// --- release / reset ------------------------------------------------------
TEST_F(UniquePtrTest, ReleaseGivesUpOwnershipWithoutDeleting) {
    EXPECT_EQ(Tracked::alive, 0);
    Tracked* raw = nullptr;
    {
        UniquePtr<Tracked> p(new Tracked(9));
        raw = p.release();
        EXPECT_EQ(p.get(), nullptr);    // pointer no longer owns it
        EXPECT_EQ(Tracked::alive, 1);   // object still alive after scope...
    }
    EXPECT_EQ(Tracked::alive, 1);       // ...because release() skipped delete
    delete raw;                          // caller must clean up now
    EXPECT_EQ(Tracked::alive, 0);
}

TEST_F(UniquePtrTest, ResetDeletesOldAndAdoptsNew) {
    EXPECT_EQ(Tracked::alive, 0);
    UniquePtr<Tracked> p(new Tracked(1));
    p.reset(new Tracked(2));            // old freed, new adopted
    EXPECT_EQ(Tracked::alive, 1);
    EXPECT_EQ(p->value, 2);
}

TEST_F(UniquePtrTest, ResetToNullEmpties) {
    EXPECT_EQ(Tracked::alive, 0);
    UniquePtr<Tracked> p(new Tracked(1));
    p.reset();                          // default arg = nullptr
    EXPECT_EQ(p.get(), nullptr);
    EXPECT_EQ(Tracked::alive, 0);
}
