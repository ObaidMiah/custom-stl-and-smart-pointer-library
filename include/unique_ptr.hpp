#ifndef UNIQUE_PTR_HPP
#define UNIQUE_PTR_HPP

template<typename T>
UniquePtr<T>::UniquePtr() noexcept : m_ptr(nullptr) {}

template<typename T>
UniquePtr<T>::UniquePtr(T* ptr) noexcept : m_ptr(ptr) {}

template<typename T>
UniquePtr<T>::~UniquePtr() {
    // TODO: delete m_ptr (delete nullptr is safe)
}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept : m_ptr(nullptr) {
    // TODO: take other.m_ptr, then set other.m_ptr to nullptr
    (void)other;
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
    // TODO: guard self-assignment, delete current m_ptr, steal other's, null other
    (void)other;
    return *this;
}

template<typename T>
T& UniquePtr<T>::operator*() const {
    // TODO: return the owned object (*m_ptr)
    return *m_ptr;
}

template<typename T>
T* UniquePtr<T>::operator->() const noexcept {
    // TODO: return m_ptr so callers can reach members
    return m_ptr;
}

template<typename T>
T* UniquePtr<T>::get() const noexcept {
    return m_ptr;
}

template<typename T>
T* UniquePtr<T>::release() noexcept {
    // TODO: stop owning without deleting: save m_ptr, null it, return the saved ptr
    return nullptr;
}

template<typename T>
void UniquePtr<T>::reset(T* ptr) noexcept {
    // TODO: delete the current object, then adopt ptr
    (void)ptr;
}

template<typename T>
UniquePtr<T>::operator bool() const noexcept {
    return m_ptr != nullptr;
}

#endif // UNIQUE_PTR_HPP
