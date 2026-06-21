#ifndef LIST_HPP
#define LIST_HPP

// ---------------------------------------------------------------------------
// Iterator
// ---------------------------------------------------------------------------
template<typename T>
List<T>::Iterator::Iterator(Node* node) : m_node(node) {}

template<typename T>
T& List<T>::Iterator::operator*() const {
    // TODO: return the element this iterator points at
    return m_node->data;
}

template<typename T>
T* List<T>::Iterator::operator->() const {
    // TODO: return a pointer to the element
    return &m_node->data;
}

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    // TODO: advance m_node to m_node->next, then return *this
    return *this;
}

template<typename T>
typename List<T>::Iterator List<T>::Iterator::operator++(int) {
    // TODO: copy *this, advance, return the copy
    Iterator tmp = *this;
    return tmp;
}

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator--() {
    // TODO: move m_node to m_node->prev, then return *this
    return *this;
}

template<typename T>
typename List<T>::Iterator List<T>::Iterator::operator--(int) {
    // TODO: copy *this, step back, return the copy
    Iterator tmp = *this;
    return tmp;
}

template<typename T>
bool List<T>::Iterator::operator==(const Iterator& other) const {
    // TODO: two iterators are equal when they point at the same node
    return m_node == other.m_node;
}

template<typename T>
bool List<T>::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

// ---------------------------------------------------------------------------
// List
// ---------------------------------------------------------------------------
template<typename T>
List<T>::List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

template<typename T>
List<T>::List(const List& other) : m_head(nullptr), m_tail(nullptr), m_size(0) {
    // TODO: walk other's nodes and push_back a copy of each
    (void)other;
}

template<typename T>
List<T>::List(List&& other) noexcept : m_head(nullptr), m_tail(nullptr), m_size(0) {
    // TODO: steal other's head/tail/size, then null other out
    (void)other;
}

template<typename T>
List<T>::~List() {
    clear();
}

template<typename T>
List<T>& List<T>::operator=(const List& other) {
    // TODO: clear, then deep-copy other (guard against self-assignment)
    (void)other;
    return *this;
}

template<typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
    // TODO: clear, steal other's resources, null other out (guard self-assignment)
    (void)other;
    return *this;
}

template<typename T>
T& List<T>::front() {
    // TODO: throw std::runtime_error if empty, else return m_head->data
    throw std::logic_error("List::front not implemented");
}

template<typename T>
T& List<T>::back() {
    // TODO: throw std::runtime_error if empty, else return m_tail->data
    throw std::logic_error("List::back not implemented");
}

template<typename T>
void List<T>::push_front(const T& value) {
    // TODO: new node, wire it before m_head, fix m_head (and m_tail if first), ++m_size
    (void)value;
}

template<typename T>
void List<T>::push_back(const T& value) {
    // TODO: new node, wire it after m_tail, fix m_tail (and m_head if first), ++m_size
    (void)value;
}

template<typename T>
void List<T>::pop_front() {
    // TODO: unlink m_head, delete it, fix m_head/m_tail, --m_size (no-op if empty)
}

template<typename T>
void List<T>::pop_back() {
    // TODO: unlink m_tail, delete it, fix m_tail/m_head, --m_size (no-op if empty)
}

template<typename T>
typename List<T>::Iterator List<T>::insert(Iterator pos, const T& value) {
    // TODO: splice a new node before pos.m_node, return Iterator(newNode)
    (void)value;
    return pos;
}

template<typename T>
typename List<T>::Iterator List<T>::erase(Iterator pos) {
    // TODO: unlink pos.m_node, delete it, return Iterator to the following node
    return pos;
}

template<typename T>
bool List<T>::empty() const noexcept {
    return m_size == 0;
}

template<typename T>
size_t List<T>::size() const noexcept {
    return m_size;
}

template<typename T>
void List<T>::clear() noexcept {
    // TODO: walk from m_head deleting each node, then reset head/tail/size
}

template<typename T>
typename List<T>::Iterator List<T>::begin() noexcept {
    return Iterator(m_head);
}

template<typename T>
typename List<T>::Iterator List<T>::end() noexcept {
    return Iterator(nullptr);
}

#endif // LIST_HPP
