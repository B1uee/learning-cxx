#include "../exercise.h"
#include <cstring>
#include <vector>

// READ: std::vector <https://zh.cppreference.com/w/cpp/container/vector>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::vector<int> vec{1, 2, 3, 4, 5};
        ASSERT(vec.size() == 5, "Fill in the correct value.");
        // THINK: `std::vector` 的大小是什么意思？与什么有关？
        // ANSWER: std::vector的大小是指容器中元素的个数，与容器中元素的个数有关
        ASSERT(sizeof(vec) == 24, "Fill in the correct value."); //vector的大小是24是因为vector是一个类模板，它的大小是由类的成员变量决定的，vector的成员变量有三个，分别是指向容器的起始地址的指针，指向容器的末尾地址的指针，指向容器的末尾地址的指针，所以vector的大小是24
        int ans[]{1, 2, 3, 4, 5};
        ASSERT(std::memcmp(&vec[0], ans, sizeof(ans)) == 0, "Fill in the correct values.");
    }
    {
        std::vector<double> vec{1, 2, 3, 4, 5};
        {
            ASSERT(vec.size() == 5, "Fill in the correct value.");
            ASSERT(sizeof(vec) == 24, "Fill in the correct value.");
            double ans[]{1, 2, 3, 4, 5};
            ASSERT(std::memcmp(vec.data(), ans, sizeof(ans)) == 0, "Fill in the correct values.");  //vec.data() 返回的是指向 std::vector 内部存储数组的指针
        }
        {
            vec.push_back(6);
            ASSERT(vec.size() == 6, "Fill in the correct value.");
            ASSERT(sizeof(vec) == 24, "Fill in the correct value.");
            vec.pop_back();
            ASSERT(vec.size() == 5, "Fill in the correct value.");
            ASSERT(sizeof(vec) == 24, "Fill in the correct value.");
        }
        {
            vec[4] = 6;
            ASSERT(vec[0] == 1, "Fill in the correct value.");
            ASSERT(vec[1] == 2, "Fill in the correct value.");
            ASSERT(vec[2] == 3, "Fill in the correct value.");
            ASSERT(vec[3] == 4, "Fill in the correct value.");
            ASSERT(vec[4] == 6, "Fill in the correct value.");
        }
        {
            // THINK: `std::vector` 插入删除的时间复杂度是什么？
            // ANSWER: 插入删除的时间复杂度是O(n)
            vec.insert(vec.begin()+1, 1.5);
            ASSERT((vec == std::vector<double>{1, 1.5, 2, 3, 4, 6}), "Make this assertion pass.");
            vec.erase(vec.begin()+ 3);
            ASSERT((vec == std::vector<double>{1, 1.5, 2, 4, 6}), "Make this assertion pass.");
        }
        {
            vec.shrink_to_fit(); // shrink_to_fit() 减少容器的容量以匹配其大小
            ASSERT(vec.capacity() == 5, "Fill in the correct value.");
            vec.clear(); // clear() 删除容器中的所有元素
            ASSERT(vec.empty(), "`vec` is empty now.");
            ASSERT(vec.size() == 0, "Fill in the correct value.");
            ASSERT(vec.capacity() == 5, "Fill in the correct value.");
        }
    }
    {
        std::vector<char> vec(48, 'z'); // TODO: 调用正确的构造函数
        ASSERT(vec[0] == 'z', "Make this assertion pass.");
        ASSERT(vec[47] == 'z', "Make this assertion pass.");
        ASSERT(vec.size() == 48, "Make this assertion pass.");
        ASSERT(sizeof(vec) == 24, "Fill in the correct value.");
        {
            auto capacity = vec.capacity();
            vec.resize(16); // resize() 改变容器的大小，使其包含指定数量的元素,如果当前大小大于n，则删除超出的元素，如果当前大小小于n，则添加元素，对capacity()的值没有影响
            ASSERT(vec.size() == 16, "Fill in the correct value.");
            ASSERT(vec.capacity() == 48, "Fill in a correct identifier.");
        }
        {
            vec.reserve(256); // reserve() 请求更改容器的容量，如果n大于当前容量，则分配新存储，否则不做任何事情
            ASSERT(vec.size() == 16, "Fill in the correct value.");
            ASSERT(vec.capacity() == 256, "Fill in the correct value.");
        }
        {
            // 此时vec已经经过了resize(16)和reserve(256)操作
            vec.push_back('a');
            vec.push_back('b');
            vec.push_back('c');
            vec.push_back('d');

            ASSERT(vec.size() == 20, "Fill in the correct value.");
            ASSERT(vec.capacity() == 256, "Fill in the correct value.");
            ASSERT(vec[15] == 'z', "Fill in the correct value.");
            ASSERT(vec[48] == NULL, "Fill in the correct value."); // vec[48]是不存在的，所以返回NULL
            ASSERT(vec[49] == NULL, "Fill in the correct value.");
            ASSERT(vec[50] == NULL, "Fill in the correct value.");
            ASSERT(vec[51] == NULL, "Fill in the correct value.");
        }
    }

    return 0;
}
