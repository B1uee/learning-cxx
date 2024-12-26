#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // TODO: 正确初始化静态字段
    static int num_a;   

    A() {
        ++num_a;
    }
    ~A() {
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};
struct B final : public A {
    // TODO: 正确初始化静态字段
    static int num_b;

    B() { // 相当于B() : A() {}
        ++num_b;
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};

int A::num_a = 0;
int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A; // new A() 会调用A的构造函数
    auto b = new B; // new B() 会调用A的构造函数和B的构造函数
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B;// 派生类指针可以随意转换为基类指针
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correctß value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    B& bb = dynamic_cast<B&>(*ab); // 引用转换，功能是将基类引用转换为派生类引用，这里的*是解引用操作符

    // B* bb = dynamic_cast<B*>(ab);  指针转换，但由于后面用的是bb.name()，所以bb应该是对象本身或者引用
    /*
        cpp有四种转换类型：
        1. static_cast：静态转换，编译时确定，不会进行运行时检查，不安全，主要用于基本类型之间的转换
        2. dynamic_cast：动态转换，运行时确定，会进行运行时检查，安全，主要用于类层次结构之间的转换
        3. const_cast：去除const属性
        4. reinterpret_cast：重新解释类型，不安全，主要用于指针之间的转换，如void*指针转换为其他指针
    */
    
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab;// 通过指针可以删除指向的对象，即使是多态对象
    ASSERT(A::num_a == 0, "Every A was destroyed"); 
    ASSERT(B::num_b == 1, "Every B was destroyed");  // A的析构函数不是虚函数，所以只会调用A的析构函数，不会调用B的析构函数

    return 0;
}
