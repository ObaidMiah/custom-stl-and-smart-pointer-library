#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <cstddef>
#include <utility>

// Exclusive-ownership smart pointer: exactly one UniquePtr owns the object and
// frees it on destruction. Copying is forbidden; ownership only moves.
template<typename T>
class UniquePtr {
public:
    UniquePtr() noexcept;                          // Owns nothing
    explicit UniquePtr(T* ptr) noexcept;           // Takes ownership of ptr
    ~UniquePtr();                                  // Deletes the owned object

    UniquePtr(const UniquePtr&) = delete;            // No copying...
    UniquePtr& operator=(const UniquePtr&) = delete; // ...unique means unique

    UniquePtr(UniquePtr&& other) noexcept;           // Transfer ownership
    UniquePtr& operator=(UniquePtr&& other) noexcept;// Transfer ownership

    T& operator*() const;                          // Access the object
    T* operator->() const noexcept;                // Member access
    T* get() const noexcept;                        // Raw pointer (no transfer)

    T* release() noexcept;                          // Give up ownership, return ptr
    void reset(T* ptr = nullptr) noexcept;          // Delete current, adopt ptr

    explicit operator bool() const noexcept;        // True if it owns something

private:
    T* m_ptr;   // The owned object (or nullptr)
};

// Implementation
#include "unique_ptr.hpp"

#endif // UNIQUE_PTR_H
