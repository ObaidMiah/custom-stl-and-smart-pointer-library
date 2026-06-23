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
    if(node == nullptr)
    {
        inserted = true;
        return new Node(key, value);
    }

    if(key < node->key)
    {
        node->left = insert(node->left, key, value, inserted);
    }
    else if(key > node->key)
    {
        node->right = insert(node->right, key, value, inserted);
    }
    else
    {
        // Key already present: operator[] must NOT overwrite the existing value.
        inserted = false;
    }

    return node;
}

template<typename K, typename V>
typename Map<K, V>::Node*
Map<K, V>::find(Node* node, const K& key) const {
    if(node == nullptr)
    {
        return nullptr;          // key not in the tree
    }

    if(key == node->key)
    {
        return node;
    }
    else if(key < node->key)
    {
        return find(node->left, key);
    }
    else
    {
        return find(node->right, key);
    }
}

template<typename K, typename V>
typename Map<K, V>::Node*
Map<K, V>::erase(Node* node, const K& key, bool& removed) {
    if(node == nullptr)
    {
        removed = false;         // key not found
        return nullptr;
    }

    if(key < node->key)
    {
        node->left = erase(node->left, key, removed);
    }
    else if(key > node->key)
    {
        node->right = erase(node->right, key, removed);
    }
    else
    {
        // Found the node to remove.
        removed = true;

        // Case 1: no left child -> splice in the right child (may be null).
        if(node->left == nullptr)
        {
            Node* child = node->right;
            delete node;
            return child;
        }
        // Case 2: no right child -> splice in the left child.
        if(node->right == nullptr)
        {
            Node* child = node->left;
            delete node;
            return child;
        }

        // Case 3: two children -> copy the in-order successor (smallest key in
        // the right subtree, which has no left child), then erase it from there.
        Node* succ = node->right;
        while(succ->left != nullptr)
        {
            succ = succ->left;
        }
        node->key   = succ->key;
        node->value = succ->value;

        bool dummy = false;      // don't let this clobber 'removed' (already true)
        node->right = erase(node->right, succ->key, dummy);
    }

    return node;
}

template<typename K, typename V>
void Map<K, V>::destroy(Node* node) noexcept {
    if(node == nullptr)
    {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ---------------------------------------------------------------------------
// Public API (mostly thin wrappers over the helpers above)
// ---------------------------------------------------------------------------
template<typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    bool inserted = false;
    // Insert a default-constructed value if the key is missing; leave an
    // existing value untouched.
    m_root = insert(m_root, key, V(), inserted);
    if(inserted)
    {
        ++m_size;
    }
    return find(m_root, key)->value;   // guaranteed to exist now
}

template<typename K, typename V>
V& Map<K, V>::at(const K& key) {
    Node* node = find(m_root, key);
    if(node == nullptr)
    {
        throw std::out_of_range("Map::at key not found");
    }
    return node->value;
}

template<typename K, typename V>
bool Map<K, V>::contains(const K& key) const {
    return find(m_root, key) != nullptr;
}

template<typename K, typename V>
bool Map<K, V>::erase(const K& key) {
    bool removed = false;
    m_root = erase(m_root, key, removed);
    if(removed)
    {
        --m_size;
    }
    return removed;
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
    destroy(m_root);
    m_root = nullptr;
    m_size = 0;
}

#endif // MAP_HPP
