#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

// ===========================================================================
// HOW TO READ THIS FILE
//   * Functions WITHOUT a "// TODO" are complete — trivial one-liners.
//   * Functions WITH a "// TODO" are yours to implement.
//   * Most methods here are noexcept, so the stubs CANNOT throw "not
//     implemented" — they silently no-op or return 0/empty. That means an
//     unfinished method leaks or reports wrong counts rather than crashing,
//     so test with the instance-counter + use_count() assertions (and ASan).
//   * The heart of the whole class is release() — getting the
//     "delete object vs. delete control block" split right.
// ===========================================================================

// ---------------------------------------------------------------------------
// SharedPtr
// ---------------------------------------------------------------------------
template <typename T>
SharedPtr<T>::SharedPtr() noexcept : m_ctrl(nullptr) {}

template <typename T>
SharedPtr<T>::SharedPtr(T *ptr) : m_ctrl(nullptr)
{
    if (ptr != nullptr)
    {
        m_ctrl = new ControlBlock(ptr);
    }
}

template <typename T>
SharedPtr<T>::~SharedPtr()
{
    release();
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr &other) noexcept : m_ctrl(other.m_ctrl)
{
    if (m_ctrl != nullptr)
    {
        ++m_ctrl->shared_count;
    }
}

template <typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &other) noexcept
{
    if (this != &other)
    {
        release();
        m_ctrl = other.m_ctrl;

        if (m_ctrl)
        {
            ++m_ctrl->shared_count;
        }
    }

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr &&other) noexcept : m_ctrl(other.m_ctrl)
{
    other.m_ctrl = nullptr;
}

template <typename T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr &&other) noexcept
{
    if (this != &other)
    {
        release();
        m_ctrl = other.m_ctrl;
        other.m_ctrl = nullptr;
    }

    return *this;
}

template <typename T>
T &SharedPtr<T>::operator*() const
{
    return *m_ctrl->ptr;
}

template <typename T>
T *SharedPtr<T>::operator->() const noexcept
{
    return m_ctrl ? m_ctrl->ptr : nullptr;
}

template <typename T>
T *SharedPtr<T>::get() const noexcept
{
    return m_ctrl ? m_ctrl->ptr : nullptr;
}

template <typename T>
size_t SharedPtr<T>::use_count() const noexcept
{
    return m_ctrl ? m_ctrl->shared_count : 0;
}

template <typename T>
SharedPtr<T>::operator bool() const noexcept
{
    return get() != nullptr;
}

template <typename T>
void SharedPtr<T>::reset() noexcept
{
    release();
    m_ctrl = nullptr;
}

template <typename T>
void SharedPtr<T>::release() noexcept
{
    if (m_ctrl == nullptr)
        return;

    --m_ctrl->shared_count;

    if (m_ctrl->shared_count == 0)
    {
        delete m_ctrl->ptr;
        
        --m_ctrl->weak_count; 

        if (m_ctrl->weak_count == 0)
            delete m_ctrl;
    }

    m_ctrl = nullptr;
}

// ---------------------------------------------------------------------------
// WeakPtr
// ---------------------------------------------------------------------------
template <typename T>
WeakPtr<T>::WeakPtr() noexcept : m_ctrl(nullptr) {}

template <typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T> &shared) noexcept : m_ctrl(shared.m_ctrl)
{
    if (m_ctrl)
    {
        ++m_ctrl->weak_count;
    }
}

template <typename T>
WeakPtr<T>::~WeakPtr()
{
    release();
}

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr &other) noexcept : m_ctrl(other.m_ctrl)
{
    if (m_ctrl)
    {
        ++m_ctrl->weak_count;
    }
}

template <typename T>
WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr &other) noexcept
{
    if (this != &other)
    {
        release();
        m_ctrl = other.m_ctrl;
        
        if(m_ctrl)
        {
            ++m_ctrl->weak_count;
        }
    }
    return *this;
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr &&other) noexcept : m_ctrl(other.m_ctrl)
{
    other.m_ctrl = nullptr;
}

template <typename T>
WeakPtr<T> &WeakPtr<T>::operator=(WeakPtr &&other) noexcept
{
    if (this != &other)
    {
        release();
        m_ctrl = other.m_ctrl;
        other.m_ctrl = nullptr;
    }
    return *this;
}

template <typename T>
bool WeakPtr<T>::expired() const noexcept
{
    return use_count() == 0;
}

template <typename T>
size_t WeakPtr<T>::use_count() const noexcept
{
    return m_ctrl ? m_ctrl->shared_count : 0;
}

template <typename T>
SharedPtr<T> WeakPtr<T>::lock() const noexcept
{
    if (!expired())
    {
        auto sp = SharedPtr<T>();
        sp.m_ctrl = m_ctrl;
        ++m_ctrl->shared_count;
        return sp;
    }

    return SharedPtr<T>();
}

template <typename T>
void WeakPtr<T>::release() noexcept
{
    if (m_ctrl == nullptr)
        return;

    --m_ctrl->weak_count;

    if (m_ctrl->weak_count == 0)
    {
        delete m_ctrl;
    }

    m_ctrl = nullptr;
}

#endif // SHARED_PTR_HPP
