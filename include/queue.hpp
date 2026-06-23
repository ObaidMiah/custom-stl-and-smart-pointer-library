#ifndef QUEUE_HPP
#define QUEUE_HPP

template<typename T>
void Queue<T>::push(const T& value) {
    m_data.push_back(value);
}

template<typename T>
void Queue<T>::pop() {
    m_data.pop_front(); 
}

template<typename T>
T& Queue<T>::front() {
    return m_data.front();
}

template<typename T>
T& Queue<T>::back() {
    return m_data.back();
}

template<typename T>
bool Queue<T>::empty() const noexcept {
    return m_data.empty();
}

template<typename T>
size_t Queue<T>::size() const noexcept {
    return m_data.size();
}

#endif // QUEUE_HPP
