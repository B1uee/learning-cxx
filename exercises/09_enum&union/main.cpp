#include "../exercise.h"

// READ: 枚举类型 <https://zh.cppreference.com/w/cpp/language/enum>

// `enum` 是 C 的兼容类型，本质上其对应类型的常量。
// 在 `enum` 中定义标识符等价于定义 constexpr 常量，
// 这些标识符不需要前缀，可以直接引用。
// 因此 `enum` 定义会污染命名空间。
enum ColorEnum : unsigned char {
    COLOR_RED = 31,
    COLOR_GREEN, // 隐式赋值为 COLOR_RED + 1
    COLOR_YELLOW,
    COLOR_BLUE,
};

// 有作用域枚举型是 C++ 引入的类型安全枚举。
// 其内部标识符需要带前缀引用，如 `Color::Red`。
// 作用域枚举型可以避免命名空间污染，并提供类型安全保证。
enum class Color : int {
    Red = COLOR_RED,
    Green,
    Yellow,
    Blue,
};

ColorEnum convert_by_pun(Color c) {
    // READ: <https://zh.cppreference.com/w/cpp/language/union>
    // `union` 表示在同一内存位置存储的不同类型的值。
    // 其常见用法是实现类型双关转换，即将一种类型的值转换为另一种无关类型的值。
    // 但这种写法实际上仅在 C 语言良定义，在 C++ 中是未定义行为。
    // 这是比较少见的 C++ 不与 C 保持兼容的特性。
    // READ: 类型双关 <https://tttapa.github.io/Pages/Programming/Cpp/Practices/type-punning.html>
    // 所有非静态数据成员均拥有相同的地址，联合体的大小是其最大成员的大小
    union TypePun {
        ColorEnum e;
        Color c;
    };

    TypePun pun;
    // TODO: 补全类型双关转换
    pun.c = c;  //通过联合体的内存重用特性，将一个类型的数据解释为另一种类型。这种方法称为类型双关转换

    // 假设我想实现逐比特将float转为int的功能
    // float f = 1.0;
    // int i = *reinterpret_cast<int*>(&f);
    // 这样的写法在大多数平台上都是可以正常工作的，因为都是4byte，但它其实是未定义行为，
    // x86支持非对齐内存访问，但是ARM和RISCV不支持，所以这样的写法在ARM上可能会出现问题

    return pun.e;
}

int main(int argc, char **argv) {
    ASSERT(convert_by_pun(Color::Red) == COLOR_RED, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Green) == COLOR_GREEN, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Yellow) == COLOR_YELLOW, "Type punning conversion");
    ASSERT(convert_by_pun(Color::Blue) == COLOR_BLUE, "Type punning conversion");
    return 0;
}
