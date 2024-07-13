#ifndef __UNIQUEPTR__
#define __UNIQUEPTR__

#include <utility>

namespace Custom {

template <typename T>
class UniquePtr {
public:
    UniquePtr() = default;

    template<typename ... Ts>
    UniquePtr(Ts&& ... ts) {
        _ptr = new T(std::forward(ts...));
    }

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

    [[nodiscard]] T* Releast() noexcept {
        auto ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    auto operator*() -> decltype(auto) requires { *_ptr; }  {
        return *_ptr;
    }

    auto operator->() -> decltype(auto) requires { _ptr.operator=(); } {
        return _ptr;
    }

    [[nodiscard]] auto operator bool() -> decltype(auto) const noexcept {
        return _ptr != nullptr;
    }

    virtual ~UniquePtr() noexcept { delete _ptr; }

protected:
    T* _ptr{nullptr};
};

}

#endif  // __UNIQUEPTR__