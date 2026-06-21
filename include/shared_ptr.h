#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <cstddef>
#include <utility>

template<typename T> class WeakPtr;

// Shared control block: lives on the heap alongside the managed object and is
// shared by every SharedPtr/WeakPtr that refers to it.
//   - shared_count drops to 0  -> delete the managed object
//   - weak_count   drops to 0  -> delete the control block itself
template<typename T>
struct ControlBlock {
    T* ptr;                // The managed object
    size_t shared_count;   // Number of owning SharedPtrs
    size_t weak_count;     // Number of observing WeakPtrs

    explicit ControlBlock(T* p) : ptr(p), shared_count(1), weak_count(0) {}
};

// Reference-counted shared ownership: the object is freed when the last
// SharedPtr to it is destroyed.
template<typename T>
class SharedPtr {
public:
    SharedPtr() noexcept;                            // Owns nothing
    explicit SharedPtr(T* ptr);                      // Take ownership, count = 1
    ~SharedPtr();                                    // Decrement, free if last

    SharedPtr(const SharedPtr& other) noexcept;            // Share: ++shared_count
    SharedPtr& operator=(const SharedPtr& other) noexcept; // Share: release old, ++new

    SharedPtr(SharedPtr&& other) noexcept;                 // Steal, no count change
    SharedPtr& operator=(SharedPtr&& other) noexcept;      // Steal, release old

    T& operator*() const;                            // Access the object
    T* operator->() const noexcept;                  // Member access
    T* get() const noexcept;                          // Raw pointer
    size_t use_count() const noexcept;                // Current shared_count
    explicit operator bool() const noexcept;          // True if it owns something

    void reset() noexcept;                            // Release ownership

private:
    ControlBlock<T>* m_ctrl;   // Shared bookkeeping (nullptr when empty)

    void release() noexcept;   // Drop one shared ref, free object/block as needed

    friend class WeakPtr<T>;   // WeakPtr::lock() rebuilds a SharedPtr from a block
};

// Non-owning observer of a SharedPtr-managed object. Does not keep the object
// alive; call lock() to obtain a SharedPtr if the object still exists.
template<typename T>
class WeakPtr {
public:
    WeakPtr() noexcept;                              // Observes nothing
    WeakPtr(const SharedPtr<T>& shared) noexcept;    // Observe shared's object
    ~WeakPtr();                                      // Decrement weak_count

    WeakPtr(const WeakPtr& other) noexcept;
    WeakPtr& operator=(const WeakPtr& other) noexcept;

    WeakPtr(WeakPtr&& other) noexcept;
    WeakPtr& operator=(WeakPtr&& other) noexcept;

    bool expired() const noexcept;                   // Has the object been freed?
    size_t use_count() const noexcept;               // Owning SharedPtrs remaining
    SharedPtr<T> lock() const noexcept;              // SharedPtr if alive, else empty

private:
    ControlBlock<T>* m_ctrl;   // Same block the SharedPtr uses (nullptr when empty)

    void release() noexcept;   // Drop one weak ref, free block if fully unused
};

// Implementation
#include "shared_ptr.hpp"

#endif // SHARED_PTR_H
