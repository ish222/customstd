#ifndef __ThreadSafeSharedPtr__
#define __ThreadSafeSharedPtr__

#include <atomic>
#include <utility>
#include <iostream>

namespace Custom {

template <typename T>
class ThreadSafeSharedPtr {
public:
    ThreadSafeSharedPtr() noexcept = default;
    ThreadSafeSharedPtr(T* p_ptr) noexcept : _ptr{p_ptr}, _count{new std::atomic_int{1}} {}
    
    virtual ~ThreadSafeSharedPtr() noexcept { DecrementCount(); }

    ThreadSafeSharedPtr(const ThreadSafeSharedPtr& p_other) noexcept {
        Copy(p_other);
    }

    ThreadSafeSharedPtr& operator=(const ThreadSafeSharedPtr& p_other) noexcept {
        if (this != &p_other) {
            Copy(p_other);
        }

        return *this;
    }

    ThreadSafeSharedPtr(ThreadSafeSharedPtr&& p_other) noexcept {
        MoveAndReset(p_other);
    }

    ThreadSafeSharedPtr& operator=(ThreadSafeSharedPtr&& p_other) noexcept {
        if (this != &p_other) {
            MoveAndReset(p_other);
        }

        return *this;
    }

    template<class Self>
    auto&& Get(this Self&& p_self) noexcept {
        return std::forward<Self>(p_self)._ptr;
    }

    void Reset() noexcept {
        DecrementCount();
        _ptr = nullptr;
        _count = nullptr;
    }

    void Reset(T* p_ptr) noexcept {
        DecrementCount();
        _ptr = p_ptr;
        _count = _ptr != nullptr ? new std::atomic_int{1} : nullptr;
    }

    void Swap(ThreadSafeSharedPtr& p_other) noexcept {
        std::swap(_ptr, p_other._ptr);
        std::swap(_count, p_other._count);
    }

    [[nodiscard]] int32_t Count() const noexcept { if (_count) [[likely]] return _count->load(); return {}; }

    [[nodiscard]] operator bool() const noexcept { return _ptr != nullptr; }

    template<class Self>
    auto&& operator*(this Self&& p_self) noexcept {
        return *(std::forward<Self>(p_self)._ptr);
    }

    template<class Self>
    auto&& operator->(this Self&& p_self) noexcept {
        return std::forward<Self>(p_self)._ptr;
    }

protected:
    T* _ptr{nullptr};
    std::atomic_int* _count{nullptr};

    void DecrementCount() noexcept {
        if (_count && --*_count == 0) {
            DeleteThis();
        }
    }

    void DeleteThis() noexcept {
        delete _ptr;
        _ptr = nullptr;
        _count = nullptr;
        std::cout << "Shared ptr deleted!\n";
    }

    void Copy(const ThreadSafeSharedPtr& p_other) noexcept {
        if (_ptr)
            DecrementCount();

        _ptr = p_other._ptr;
        _count = p_other._count;
        if (_ptr)
            ++*_count;

        std::cout << "Shared ptr copied!\n";
    }

    void MoveAndReset(ThreadSafeSharedPtr&& p_other) noexcept {
        DecrementCount();
        _ptr = p_other._ptr;
        _count = p_other._count;
        p_other._ptr = nullptr;
        p_other._count = 0;
        std::cout << "Shared ptr moved!\n";
    }
};

}

#endif