#ifndef LIST_HPP
#define LIST_HPP

// ===========================================================================
// HOW TO READ THIS FILE
//   * Functions WITHOUT a "// TODO" are complete — trivial one-liners that
//     don't teach anything; leave them as-is.
//   * Functions WITH a "// TODO" are yours to implement. Their bodies either
//     throw "not implemented" or do nothing, so the file compiles but any
//     unfinished piece fails loudly instead of silently misbehaving.
//   * insert()/erase() are marked OPTIONAL — skip them until the rest works.
// ===========================================================================

// ---------------------------------------------------------------------------
// Iterator
// ---------------------------------------------------------------------------
template <typename T>
List<T>::Iterator::Iterator(Node *node) : m_node(node) {}

template <typename T>
T &List<T>::Iterator::operator*() const
{
    return m_node->data;
}

template <typename T>
T *List<T>::Iterator::operator->() const
{
    return &m_node->data;
}

template <typename T>
typename List<T>::Iterator &List<T>::Iterator::operator++()
{
    m_node = m_node->next;
    return *this;
}

template <typename T>
typename List<T>::Iterator List<T>::Iterator::operator++(int)
{
    Node *temp = m_node;
    m_node = m_node->next;
    return Iterator(temp);
}

template <typename T>
typename List<T>::Iterator &List<T>::Iterator::operator--()
{
    m_node = m_node->prev;
    return *this;
}

template <typename T>
typename List<T>::Iterator List<T>::Iterator::operator--(int)
{
    Node *temp = m_node;
    m_node = m_node->prev;
    return Iterator(temp);
}

template <typename T>
bool List<T>::Iterator::operator==(const Iterator &other) const
{
    return m_node == other.m_node;
}

template <typename T>
bool List<T>::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

// ---------------------------------------------------------------------------
// List — construction / destruction
// ---------------------------------------------------------------------------
template <typename T>
List<T>::List() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

template <typename T>
List<T>::List(const List &other) : m_head(nullptr), m_tail(nullptr), m_size(0)
{
    for (Node *cur = other.m_head; cur != nullptr; cur = cur->next)
    {
        push_back(cur->data);
    }
}

template <typename T>
List<T>::List(List &&other) noexcept : m_head(nullptr), m_tail(nullptr), m_size(0)
{
    m_head = other.m_head;
    m_tail = other.m_tail;
    m_size = other.m_size;

    other.m_head = nullptr;
    other.m_tail = nullptr;
    other.m_size = 0;
}

template <typename T>
List<T>::~List()
{
    clear();
}

template <typename T>
List<T> &List<T>::operator=(const List &other)
{
    if (this != &other) //self-assignment 
    {
        clear();

        for (Node *cur = other.m_head; cur != nullptr; cur = cur->next)
        {
            push_back(cur->data);
        }
    }

    return *this;
}

template <typename T>
List<T> &List<T>::operator=(List &&other) noexcept
{
    clear();

    m_head = other.m_head;
    m_tail = other.m_tail;
    m_size = other.m_size;

    other.m_head = nullptr;
    other.m_tail = nullptr;
    other.m_size = 0;

    return *this;
}

// ---------------------------------------------------------------------------
// List — element access
// ---------------------------------------------------------------------------
template <typename T>
T &List<T>::front()
{
    if (m_head == nullptr)
    {
        throw std::runtime_error("List::front List is empty");
    }

    return m_head->data;
}

template <typename T>
T &List<T>::back()
{
    if (m_tail == nullptr)
    {
        throw std::runtime_error("List::back List is empty");
    }

    return m_tail->data;
}

// ---------------------------------------------------------------------------
// List — modifiers
// ---------------------------------------------------------------------------
template <typename T>
void List<T>::push_front(const T &value)
{
    Node *temp = new Node(value);

    if (m_head == nullptr)
    {
        m_head = temp;
        m_tail = temp;
    }
    else
    {
        temp->next = m_head; 
        m_head->prev = temp;
        m_head = temp;
    }

    ++m_size;
}

template <typename T>
void List<T>::push_back(const T &value)
{
    Node *temp = new Node(value);

    if (m_tail == nullptr)
    {
        m_head = temp;
        m_tail = temp;
    }
    else
    {
        temp->prev = m_tail; 
        m_tail->next = temp;
        m_tail = temp;
    }

    ++m_size;
}

template <typename T>
void List<T>::pop_front()
{
    if (m_size == 0)
        return;

    Node *temp = m_head;
    m_head = m_head->next;

    if(m_head)
    {
        m_head->prev = nullptr; 
    }
    else
    {
        m_tail = nullptr;
    }

    delete temp;

    --m_size;
}

template <typename T>
void List<T>::pop_back()
{
    if (m_size == 0)
        return;

    Node *temp = m_tail;
    m_tail = m_tail->prev;

    if(m_tail)
    {
        m_tail->next = nullptr;
    }   
    else
    {
        m_head = nullptr; 
    }

    delete temp;

    --m_size;
}

// OPTIONAL — skip until the basics above work.
template <typename T>
typename List<T>::Iterator List<T>::insert(Iterator pos, const T &value)
{
    Node* cur = pos.m_node; 

    if(cur == nullptr) // end of list 
    {
        push_back(value); 
        return Iterator(m_tail); 
    }
    if(cur->prev == nullptr) // start of list 
    {
        push_front(value);
        return Iterator(m_head);
    }

    Node* prev = cur->prev; 
    Node* temp = new Node(value); 

    temp->prev = prev; 
    temp->next = cur; 

    prev->next = temp; 
    cur->prev = temp; 

    ++m_size; 
    return Iterator(temp);
}

// OPTIONAL — skip until the basics above work.
template <typename T>
typename List<T>::Iterator List<T>::erase(Iterator pos)
{
    Node* cur = pos.m_node; 

    if(cur == nullptr)
    {
        return end(); 
    }

    Node* next = cur->next; 

    if(cur->prev)
    {
        cur->prev->next = cur->next; 
    }
    else // 1 element 
    {
        m_head = cur->next; 
    }

    if(cur->next)
    {
        cur->next->prev = cur->prev; 
    }
    else // 1 element 
    {
        m_tail = cur->prev;
    }

    delete cur; 
    --m_size; 
    return Iterator(next); 
}

// ---------------------------------------------------------------------------
// List — capacity / cleanup
// ---------------------------------------------------------------------------
template <typename T>
bool List<T>::empty() const noexcept
{
    return m_size == 0;
}

template <typename T>
size_t List<T>::size() const noexcept
{
    return m_size;
}

template <typename T>
void List<T>::clear() noexcept
{
    // TODO: walk from m_head deleting each node, then reset m_head/m_tail to null and m_size to 0

    Node* cur = m_head; 

    while(cur)
    {
        Node* next = cur->next; 
        delete cur; 
        cur = next; 
    }

    m_head = nullptr; 
    m_tail = nullptr; 
    m_size = 0; 
}

// ---------------------------------------------------------------------------
// List — iterators
// ---------------------------------------------------------------------------
template <typename T>
typename List<T>::Iterator List<T>::begin() noexcept
{
    return Iterator(m_head);
}

template <typename T>
typename List<T>::Iterator List<T>::end() noexcept
{
    return Iterator(nullptr);
}

#endif // LIST_HPP
