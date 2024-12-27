#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10); // shared是一个指向int的shared_ptr
    std::shared_ptr<int> ptrs[]{shared, shared, shared}; // 创建一个 std::shared_ptr<int> 数组，包含三个指向同一对象的指针。此时，shared 的引用计数增加到 4

    std::weak_ptr<int> observer = shared;   // weak_ptr是一个弱引用，不影响shared的引用计数
    ASSERT(observer.use_count() == 4, "");

    ptrs[0].reset(); // reset() 会将指针置空
    ASSERT(observer.use_count() == 3, "");

    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 2, "");

    ptrs[2] = std::make_shared<int>(*shared); 
    // std::make_shared<int>(*shared) 创建了一个新的 std::shared_ptr，它指向一个新的整数对象，值与 *shared 相同，但与原有对象是不同的实例，因此不影响原有的计数
    // 然而ptrs[2]原来的指针被释放了，因此引用计数减少
    ASSERT(observer.use_count() == 1, "");

    ptrs[0] = shared; // 2
    ptrs[1] = shared; // 3
    ptrs[2] = std::move(shared);   // std::move(shared) 将 shared 转换为右值引用，并将其资源转移给 ptrs[2]，引用计数不变
    ASSERT(observer.use_count() == 3, "");

    std::ignore = std::move(ptrs[0]); //std::ignore 并没有接受或存储该右值引用，因此对计数无影响
    ptrs[1] = std::move(ptrs[1]);  // 3
    ptrs[1] = std::move(ptrs[2]);  // 2 释放了ptrs[1]原来的指针
    ASSERT(observer.use_count() == 2, "");

    shared = observer.lock();  // lock() 会返回一个 shared_ptr，指向weak_ptr指向的对象，如果对象已经被销毁，则返回空指针。这用于安全地从 std::weak_ptr 升级到 std::shared_ptr，以确保对象在使用时仍然存在
    ASSERT(observer.use_count() == 3, "");

    shared = nullptr; // 2
    
    for (auto &ptr : ptrs){
        ptr = nullptr;
        std::cout << observer.use_count() << std::endl;
    }
    // user_count()最少为0
    ASSERT(observer.use_count() == 0, "");

    shared = observer.lock();
    ASSERT(observer.use_count() == 0, "");

    return 0;
}
