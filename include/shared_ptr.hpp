#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

// ---------------------------------------------------------------------------
// SharedPtr
// ---------------------------------------------------------------------------
template<typename T>
SharedPtr<T>::SharedPtr() noexcept : m_ctrl(nullptr) {}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr) : m_ctrl(nullptr) {
    // TODO: if ptr, allocate a ControlBlock<T>(ptr) and store it in m_ctrl
    (void)ptr;
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    release();
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept : m_ctrl(other.m_ctrl) {
    // TODO: if m_ctrl, ++m_ctrl->shared_count
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) noexcept {
    // TODO: guard self-assignment, release() current, copy other.m_ctrl, ++shared_count
    (void)other;
    return *this;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept : m_ctrl(other.m_ctrl) {
    // TODO: null out other.m_ctrl (ownership moved, no count change)
    (void)other;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept {
    // TODO: guard self-assignment, release() current, steal other.m_ctrl, null other
    (void)other;
    return *this;
}

template<typename T>
T& SharedPtr<T>::operator*() const {
    // TODO: return *(m_ctrl->ptr)
    return *m_ctrl->ptr;
}

template<typename T>
T* SharedPtr<T>::operator->() const noexcept {
    // TODO: return m_ctrl ? m_ctrl->ptr : nullptr
    return m_ctrl ? m_ctrl->ptr : nullptr;
}

template<typename T>
T* SharedPtr<T>::get() const noexcept {
    return m_ctrl ? m_ctrl->ptr : nullptr;
}

template<typename T>
size_t SharedPtr<T>::use_count() const noexcept {
    // TODO: return m_ctrl ? m_ctrl->shared_count : 0
    return 0;
}

template<typename T>
SharedPtr<T>::operator bool() const noexcept {
    return get() != nullptr;
}

template<typename T>
void SharedPtr<T>::reset() noexcept {
    // TODO: release() then set m_ctrl to nullptr
}

template<typename T>
void SharedPtr<T>::release() noexcept {
    // TODO (core logic):
    //   if no m_ctrl, return
    //   --shared_count
    //   if shared_count == 0: delete m_ctrl->ptr;
    //       if weak_count == 0 also delete m_ctrl
    //   set m_ctrl = nullptr
}

// ---------------------------------------------------------------------------
// WeakPtr
// ---------------------------------------------------------------------------
template<typename T>
WeakPtr<T>::WeakPtr() noexcept : m_ctrl(nullptr) {}

template<typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared) noexcept : m_ctrl(shared.m_ctrl) {
    // TODO: if m_ctrl, ++m_ctrl->weak_count
    (void)shared;
}

template<typename T>
WeakPtr<T>::~WeakPtr() {
    release();
}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept : m_ctrl(other.m_ctrl) {
    // TODO: if m_ctrl, ++m_ctrl->weak_count
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
    // TODO: guard self-assignment, release() current, copy other.m_ctrl, ++weak_count
    (void)other;
    return *this;
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept : m_ctrl(other.m_ctrl) {
    // TODO: null out other.m_ctrl
    (void)other;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
    // TODO: guard self-assignment, release() current, steal other.m_ctrl, null other
    (void)other;
    return *this;
}

template<typename T>
bool WeakPtr<T>::expired() const noexcept {
    // TODO: true when there is no block or shared_count == 0
    return use_count() == 0;
}

template<typename T>
size_t WeakPtr<T>::use_count() const noexcept {
    // TODO: return m_ctrl ? m_ctrl->shared_count : 0
    return 0;
}

template<typename T>
SharedPtr<T> WeakPtr<T>::lock() const noexcept {
    // TODO: if not expired, build a SharedPtr that shares m_ctrl (++shared_count);
    //       otherwise return an empty SharedPtr.
    return SharedPtr<T>();
}

template<typename T>
void WeakPtr<T>::release() noexcept {
    // TODO (core logic):
    //   if no m_ctrl, return
    //   --weak_count
    //   if weak_count == 0 AND shared_count == 0: delete m_ctrl
    //   set m_ctrl = nullptr
}

#endif // SHARED_PTR_HPP
