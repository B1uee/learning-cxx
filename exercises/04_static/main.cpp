#include "../exercise.h"

// READ: `static` 关键字 <https://zh.cppreference.com/w/cpp/language/storage_duration>
// THINK: 这个函数的两个 `static` 各自的作用是什么？
static int func(int param) { // static 修饰函数可以使得函数只能在当前文件中使用
    // static 修饰局部变量可以使得变量在函数调用结束后不会被销毁，而是一直存在于内存中，下次调用该函数时可以继续使用
    // 因此static_ 只在首次调用时初始化，后续调用不会重新赋值为 param，而是使用上一次调用后的值
    static int static_ = param; 
    std::cout << "static_ = " << static_ << std::endl;
    return static_++;
}

int main(int argc, char **argv) {
    // TODO: 将下列 `?` 替换为正确的数字
    ASSERT(func(5) == 5, "static variable value incorrect");
    ASSERT(func(4) == 6, "static variable value incorrect");
    ASSERT(func(3) == 7, "static variable value incorrect");
    ASSERT(func(2) == 8, "static variable value incorrect");
    ASSERT(func(1) == 9, "static variable value incorrect");
    return 0;
}
