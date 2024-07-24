#include <SharedPtr.h>
#include <UniquePtr.h>

int main() {
    Custom::SharedPtr<int> ptr{new int{2}};
    std::cout << "ptr = " << *ptr << '\n';
    auto ptr2 = ptr;
    *ptr2 = 5;
    std::cout << "ptr = " << *ptr << '\n';
    std::cout << "ptr2 = " << *ptr2 << '\n';

    Custom::UniquePtr<int[]> uptr(new int[10]);
    uptr[1] = 2;
    std::cout << uptr[1];
}