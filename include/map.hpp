#ifndef MAP_HPP
#define MAP_HPP

template<typename K, typename V>
Map<K, V>::Map() : m_root(nullptr), m_size(0) {}

template<typename K, typename V>
Map<K, V>::~Map() {
    destroy(m_root);
}

// ---------------------------------------------------------------------------
// Private recursive helpers
// ---------------------------------------------------------------------------
template<typename K, typename V>
typename Map<K, V>::Node*
Map<K, V>::insert(Node* node, const K& key, const V& value, bool& inserted) {
    // TODO:
    //   if node == nullptr: set inserted = true; return new Node(key, value)
    //   if key < node->key:   node->left  = insert(node->left,  key, value, inserted)
    //   else if key > node->key: node->right = insert(node->right, key, value, inserted)
    //   else: key exists -> node->value = value; inserted = false
    //   return node
    (void)key; (void)value; (void)inserted;
    return node;
}

template<typename K, typename V>
typename Map<K, V>::Node*
Map<K, V>::find(Node* node, const K& key) const {
    // TODO: standard BST descent; return the matching node or nullptr
    (void)key;
    return node;
}

template<typename K, typename V>
typename Map<K, V>::Node*
Map<K, V>::erase(Node* node, const K& key, bool& removed) {
    // TODO (the tricky one):
    //   descend left/right as in find
    //   on match:
    //     0 or 1 child -> splice the child in, delete node
    //     2 children   -> copy the in-order successor (leftmost of right subtree)
    //                     into node, then erase that successor from node->right
    //   set removed appropriately; return the (possibly new) subtree root
    (void)key; (void)removed;
    return node;
}

template<typename K, typename V>
void Map<K, V>::destroy(Node* node) noexcept {
    // TODO: post-order — destroy(left); destroy(right); delete node;
    (void)node;
}

// ---------------------------------------------------------------------------
// Public API (mostly thin wrappers over the helpers above)
// ---------------------------------------------------------------------------
template<typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    // TODO: find key; if missing insert a default-constructed V; return the value ref
    (void)key;
    throw std::logic_error("Map::operator[] not implemented");
}

template<typename K, typename V>
V& Map<K, V>::at(const K& key) {
    // TODO: find key; throw std::out_of_range if absent, else return its value
    (void)key;
    throw std::logic_error("Map::at not implemented");
}

template<typename K, typename V>
bool Map<K, V>::contains(const K& key) const {
    // TODO: return find(m_root, key) != nullptr
    (void)key;
    return false;
}

template<typename K, typename V>
bool Map<K, V>::erase(const K& key) {
    // TODO: call the recursive erase, update m_root and m_size, return removed
    (void)key;
    return false;
}

template<typename K, typename V>
bool Map<K, V>::empty() const noexcept {
    return m_size == 0;
}

template<typename K, typename V>
size_t Map<K, V>::size() const noexcept {
    return m_size;
}

template<typename K, typename V>
void Map<K, V>::clear() noexcept {
    // TODO: destroy(m_root), then reset m_root = nullptr and m_size = 0
}

#endif // MAP_HPP
