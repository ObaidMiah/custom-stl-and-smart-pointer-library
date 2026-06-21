#ifndef MAP_H
#define MAP_H

#include <stdexcept>
#include <cstddef>
#include <utility>

// Ordered key->value map backed by a binary search tree.
//
// NOTE: this scaffold is an UNBALANCED BST for clarity — average O(log n),
// worst case O(n) on sorted input. Upgrading the rebalancing to a red-black
// tree later is a self-contained follow-up that only touches insert/erase.
template<typename K, typename V>
class Map {
private:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;

        Node(const K& k, const V& v)
            : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* m_root;   // Root of the tree (nullptr when empty)
    size_t m_size;  // Number of key/value pairs

    // Recursive helpers — implement these and the public API stays small.
    Node* insert(Node* node, const K& key, const V& value, bool& inserted);
    Node* find(Node* node, const K& key) const;     // Node with key, or nullptr
    Node* erase(Node* node, const K& key, bool& removed);
    void destroy(Node* node) noexcept;              // Post-order delete subtree

public:
    Map();                                          // Empty map
    ~Map();                                         // Free every node

    V& operator[](const K& key);                    // Access/insert (default V if new)
    V& at(const K& key);                            // Access existing (throws if absent)
    bool contains(const K& key) const;              // Is the key present?
    bool erase(const K& key);                       // Remove key, return whether it existed

    bool empty() const noexcept;                    // Is the map empty?
    size_t size() const noexcept;                   // Number of pairs
    void clear() noexcept;                          // Remove everything
};

// Implementation
#include "map.hpp"

#endif // MAP_H
