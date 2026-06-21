#ifndef QUEUE_HPP
#define QUEUE_HPP

template<typename T>
void Queue<T>::push(const T& value) {
    // TODO: m_data.push_back(value)
    (void)value;
}

template<typename T>
void Queue<T>::pop() {
    // TODO: m_data.pop_front()
}

template<typename T>
T& Queue<T>::front() {
    // TODO: return m_data.front()
    return m_data.front();
}

template<typename T>
T& Queue<T>::back() {
    // TODO: return m_data.back()
    return m_data.back();
}

template<typename T>
bool Queue<T>::empty() const noexcept {
    // TODO: return m_data.empty()
    return true;
}

template<typename T>
size_t Queue<T>::size() const noexcept {
    // TODO: return m_data.size()
    return 0;
}

#endif // QUEUE_HPP
