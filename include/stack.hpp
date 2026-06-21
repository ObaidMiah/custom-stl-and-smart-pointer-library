#ifndef STACK_HPP
#define STACK_HPP

template<typename T>
void Stack<T>::push(const T& value) {
    // TODO: m_data.push_back(value)
    (void)value;
}

template<typename T>
void Stack<T>::pop() {
    // TODO: m_data.pop_back()
}

template<typename T>
T& Stack<T>::top() {
    // TODO: return m_data.back()
    return m_data.back();
}

template<typename T>
bool Stack<T>::empty() const noexcept {
    // TODO: return m_data.empty()
    return true;
}

template<typename T>
size_t Stack<T>::size() const noexcept {
    // TODO: return m_data.size()
    return 0;
}

#endif // STACK_HPP
