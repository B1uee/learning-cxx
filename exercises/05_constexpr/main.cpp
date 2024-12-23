#include "../exercise.h"

constexpr unsigned long long fibonacci(int i) { // constexpr 函数编译时求值
    switch (i) {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            return fibonacci(i - 1) + fibonacci(i - 2);
    }
}

int main(int argc, char **argv) {
    constexpr auto FIB20 = fibonacci(20);
    ASSERT(FIB20 == 6765, "fibonacci(20) should be 6765");
    std::cout << "fibonacci(20) = " << FIB20 << std::endl;

    // TODO: 观察错误信息，修改一处，使代码编译运行
    // PS: 编译运行，但是不一定能算出结果……
    constexpr auto ANS_N = 20;
    //error: constexpr variable 'ANS' must be initialized by a constant expression 
    // - 超出了编译时求值的栈的规定深度
    // 因此将 ANS_N 从 constexpr变量改为普通变量可以编译 但运行很难
    // 直接改小 ANS_N 即可
    auto ANS = fibonacci(ANS_N);  
    std::cout << "fibonacci(" << ANS_N << ") = " << ANS << std::endl;

    return 0;
}
