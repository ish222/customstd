#ifndef __UNIQUEPTR__
#define __UNIQUEPTR__

#include <utility>

namespace Custom {

template <typename T>
class UniquePtr {
public:
    UniquePtr() noexcept = default;

    UniquePtr(T* p_ptr) noexcept : _ptr{p_ptr} {} 

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& p_other) noexcept {
        std::swap(_ptr, p_other._ptr);
    }

    UniquePtr& operator=(UniquePtr&& p_other) noexcept {
        if (this != &p_other) {
            std::swap(_ptr, p_other._ptr);
        }

        return *this;
    }

    [[nodiscard]] T* Get() const noexcept {
        return _ptr;
    }

    void Reset() noexcept {
        delete _ptr;
        _ptr = nullptr;
    }

    [[nodiscard]] T* Release() noexcept {
        auto ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    template<class Self>
    auto&& operator*(this Self&& p_self) noexcept {
        return *(std::forward<Self>(p_self)._ptr);
    }

    template<class Self>
    auto&& operator->(this Self&& p_self) noexcept {
        return std::forward<Self>(p_self)._ptr;
    }

    template<class Self>
    auto&& operator[](this Self&& p_self, size_t p_index) noexcept requires std::is_array_v<T> {
        // if constexpr (std::is_array<T>::value)
            return std::forward<Self>(p_self)._ptr[p_index]; 
    }

    [[nodiscard]] operator bool() const noexcept { return _ptr != nullptr; }

    virtual ~UniquePtr() noexcept { delete _ptr; }

protected:
    T* _ptr{nullptr};
};

template <typename T, typename ... Ts>
auto MakeUnique(Ts&& ... ts) {
    return UniquePtr(new T{std::forward<Ts>(ts)...});
}

}

#endif  // __UNIQUEPTR__