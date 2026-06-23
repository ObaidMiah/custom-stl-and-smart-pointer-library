#ifndef STACK_HPP
#define STACK_HPP

template<typename T>
void Stack<T>::push(const T& value) {
    m_data.push_back(value);
}

template<typename T>
void Stack<T>::pop() {
    m_data.pop_back();
}

template<typename T>
T& Stack<T>::top() {
    return m_data.back();
}

template<typename T>
bool Stack<T>::empty() const noexcept {
    return m_data.empty();
}

template<typename T>
size_t Stack<T>::size() const noexcept {
    return m_data.size();
}

#endif // STACK_HPP
