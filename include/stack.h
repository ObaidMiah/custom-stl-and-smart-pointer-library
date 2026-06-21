#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include "vector.h"

// LIFO adapter. A Stack is not a new container — it restricts an existing one
// (here your Vector) to push/pop/top at a single end. Every method delegates.
template<typename T>
class Stack {
public:
    void push(const T& value);     // Add to the top  (Vector::push_back)
    void pop();                    // Remove the top  (Vector::pop_back)
    T& top();                      // Inspect the top (Vector::back)
    bool empty() const noexcept;   // Is it empty?
    size_t size() const noexcept;  // Number of elements

private:
    Vector<T> m_data;   // Underlying storage; the top is the back
};

// Implementation
#include "stack.hpp"

#endif // STACK_H
