#ifndef __SHAREDPTR__
#define __SHAREDPTR__

#include <atomic>
#include <utility>

namespace Custom {

template <typename T>
class SharedPtr {
public:
    SharedPtr() noexcept = default;
    SharedPtr(T* p_ptr) noexcept : _ptr{p_ptr}, _count{new std::atomic_int{1}} {}
    
    virtual ~SharedPtr() noexcept { DecrementCount(); }

    SharedPtr(const SharedPtr& p_other) noexcept {
        Copy();
    }

    SharedPtr& operator=(const SharedPtr& p_other) noexcept {
        if (this != &p_other) {
            Copy(p_other);
        }

        return *this;
    }

    SharedPtr(SharedPtr&& p_other) noexcept {
        MoveAndReset(p_other);
    }

    SharedPtr& operator=(SharedPtr&& p_other) noexcept {
        if (this != &p_other) {
            MoveAndReset(p_other);
        }

        return *this;
    }

    template<class Self>
    auto&& Get(this Self&& self) {
        return std::forward<Self>(self)._ptr;
    }

protected:
    T* _ptr{nullptr};
    std::atomic_int* _count{nullptr};

    void DecrementCount() noexcept {
        if (_count) [[likely]] {
            if (--*_count == 0) {
                DeleteThis();
            }
        }
    }

    void DeleteThis() noexcept {
        delete _ptr;
        delete _count;
        _ptr = nullptr;
        _count = nullptr;
    }

    void Copy(const SharedPtr& p_other) noexcept {
        DecrementCount();
        _ptr = p_other._ptr;
        _count = p_other._count;
        if (_count)
            ++*_count;
    }

    void MoveAndReset(SharedPtr&& p_other) noexcept {
        DecrementCount();
        _ptr = p_other._ptr;
        _count = p_other._count;
        p_other._ptr = nullptr;
        p_other._count = nullptr;
    }
};

}

#endif