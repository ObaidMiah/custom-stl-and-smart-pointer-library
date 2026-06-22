#ifndef UNIQUE_PTR_HPP
#define UNIQUE_PTR_HPP

template<typename T>
UniquePtr<T>::UniquePtr() noexcept : m_ptr(nullptr) {}

template<typename T>
UniquePtr<T>::UniquePtr(T* ptr) noexcept : m_ptr(ptr) {}

template<typename T>
UniquePtr<T>::~UniquePtr() {
    delete m_ptr;
}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept : m_ptr(nullptr) {
    m_ptr = other.m_ptr; 
    other.m_ptr = nullptr; 
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
    if(this != &other)
    {
        T* temp = m_ptr; 
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr; 
        delete temp; 
    }

    return *this;
}

template<typename T>
T& UniquePtr<T>::operator*() const {
    return *m_ptr;
}

template<typename T>
T* UniquePtr<T>::operator->() const noexcept {
    return m_ptr;
}

template<typename T>
T* UniquePtr<T>::get() const noexcept {
    return m_ptr;
}

template<typename T>
T* UniquePtr<T>::release() noexcept {
    T* temp = m_ptr; 
    m_ptr = nullptr; 
    return temp;
}

template<typename T>
void UniquePtr<T>::reset(T* ptr) noexcept {
    delete m_ptr; 
    m_ptr = ptr; 
}

template<typename T>
UniquePtr<T>::operator bool() const noexcept {
    return m_ptr != nullptr;
}

#endif // UNIQUE_PTR_HPP
