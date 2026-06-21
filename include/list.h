#ifndef LIST_H
#define LIST_H

#include <stdexcept>
#include <cstddef>
#include <utility>

// Doubly-linked list. Unlike Vector, storage is node-based, so this is where
// you implement pointer surgery and a real bidirectional iterator class.
template<typename T>
class List {
private:
    // Each element lives in its own heap node, wired to its neighbours.
    struct Node {
        T data;
        Node* prev;
        Node* next;

        explicit Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
        explicit Node(T&& value) : data(std::move(value)), prev(nullptr), next(nullptr) {}
    };

    Node* m_head;   // First node (nullptr when empty)
    Node* m_tail;   // Last node  (nullptr when empty)
    size_t m_size;  // Number of nodes

public:
    // Bidirectional iterator wrapping a Node*. This is the main thing Vector
    // didn't teach you: a real iterator type rather than a raw T*.
    class Iterator {
    public:
        explicit Iterator(Node* node = nullptr);

        T& operator*() const;                 // Dereference to the element
        T* operator->() const;                // Member access on the element

        Iterator& operator++();               // Pre-increment  (advance, return self)
        Iterator operator++(int);             // Post-increment (advance, return old)
        Iterator& operator--();               // Pre-decrement
        Iterator operator--(int);             // Post-decrement

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        Node* m_node;
        friend class List;   // List needs the raw node for insert/erase
    };

    List();                                   // Empty list
    List(const List& other);                  // Copy constructor (deep copy)
    List(List&& other) noexcept;              // Move constructor
    ~List();                                  // Destructor

    List& operator=(const List& other);       // Copy assignment
    List& operator=(List&& other) noexcept;   // Move assignment

    T& front();                               // First element (throws if empty)
    T& back();                                // Last element  (throws if empty)

    void push_front(const T& value);          // Prepend element
    void push_back(const T& value);           // Append element
    void pop_front();                         // Remove first element
    void pop_back();                          // Remove last element

    Iterator insert(Iterator pos, const T& value); // Insert before pos, return new node
    Iterator erase(Iterator pos);                  // Remove node at pos, return next

    bool empty() const noexcept;              // Is the list empty?
    size_t size() const noexcept;             // Number of elements
    void clear() noexcept;                    // Destroy all nodes

    Iterator begin() noexcept;                // Iterator to first element
    Iterator end() noexcept;                  // Iterator one past the last (nullptr)
};

// Implementation
#include "list.hpp"

#endif // LIST_H
