#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>
#include "list.h"

// FIFO adapter. Backed by your List so both ends are O(1): enqueue at the back,
// dequeue from the front. (A Vector-backed queue would be O(n) per dequeue.)
template<typename T>
class Queue {
public:
    void push(const T& value);     // Enqueue at the back  (List::push_back)
    void pop();                    // Dequeue from front   (List::pop_front)
    T& front();                    // Next to be removed   (List::front)
    T& back();                     // Most recently added  (List::back)
    bool empty() const noexcept;   // Is it empty?
    size_t size() const noexcept;  // Number of elements

private:
    List<T> m_data;   // Underlying storage; front = head, back = tail
};

// Implementation
#include "queue.hpp"

#endif // QUEUE_H
