#include "../exercise.h"

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; i++) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。

    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        bool broadcast[4];
        for(auto i = 0; i < 4; ++i) {
            broadcast[i] = shape[i] != 1 && others.shape[i] == 1; // 单向广播，即仅支持other.data广播加到this.data
        }
        std::cout << "broadcast: " << broadcast[0] << " " << broadcast[1] << " " << broadcast[2] << " " << broadcast[3] << std::endl;
        auto dst = this->data, src = others.data;
        T* anchor[4]{src}; // 锚点数组，广播就是src数据不断重复的过程
        for(auto i_0 = 0; i_0 < shape[0]; ++i_0) {
            if(broadcast[0]) src = anchor[0];
            anchor[1] = src;
            for(auto i_1 = 0; i_1 < shape[1]; ++i_1) {
                if(broadcast[1]) src = anchor[1];
                anchor[2] = src;
                for(auto i_2 = 0; i_2 < shape[2]; ++i_2) {
                    if(broadcast[2]) src = anchor[2];
                    anchor[3] = src; // 每一维的锚点会在上一层固定，并在过程中更新
                    /*
                        以最后一个例子为例，broadcast = {0,1,1,1}
                        每当最后一层循环结束，scr会因为上一层也要broadcast，回退到anchor[2]，进而让anchor[3]再一次锚定到src，最终令src回退到anchor[3]
                        而倘若上一层不需要broadcast，src就会一直向前
                        总结：每一层会根据本层是否需要广播更新src，src用于更新下一层的锚点，初始锚点全是0
                    */
                    for(auto i_3 = 0; i_3 < shape[3]; ++i_3) {
                        if(broadcast[3]) src = anchor[3];  // 如果该维度要广播，就回退到锚点
                        std::cout << "dst: " << *dst << " src: " << *src << std::endl;
                        *dst += *src;
                        //std::cout << *dst << " ";
                        ++dst;
                        ++src;
                    }
                    std::cout << std::endl;
                }
            }
        }


        return *this;
    }
};

template<typename T>
Tensor4D(const unsigned int[4], const T*) -> Tensor4D<T>;

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        // for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
        //     std::cout << t0.data[i] << std::endl;
        // }
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
    // {
    //     // 自加测试
    //     unsigned int s0[]{1, 2, 3, 4};
    //     // clang-format off
    //     double d0[]{
    //          1,  2,  3,  4,
    //          5,  6,  7,  8,
    //          9, 10, 11, 12,

    //         13, 14, 15, 16,
    //         17, 18, 19, 20,
    //         21, 22, 23, 24};
    //     // clang-format on
    //     unsigned int s1[]{1, 2, 1, 1};
    //     double d1[]{
    //         1,

    //         2};

    //     auto t0 = Tensor4D(s0, d0);
    //     auto t1 = Tensor4D(s1, d1);
    //     t0 += t1;
    //     for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
    //         ASSERT(t0.data[i] == d0[i] + (i < 12 ? 1 : 2), "Every element of t0 should be incremented correctly after adding t1 to it.");
            
    //     }
    // }
    // {
    //     // 自加测试
    //     unsigned int s0[]{1, 2, 3, 4};
    //     // clang-format off
    //     double d0[]{
    //          1,  2,  3,  4,
    //          5,  6,  7,  8,
    //          9, 10, 11, 12,

    //         13, 14, 15, 16,
    //         17, 18, 19, 20,
    //         21, 22, 23, 24};
    //     // clang-format on
    //     unsigned int s1[]{1, 1, 3, 1};
    //     double d1[]{
    //         1,
    //         2,
    //         3};

    //     auto t0 = Tensor4D(s0, d0);
    //     auto t1 = Tensor4D(s1, d1);
    //     t0 += t1;
    //     for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
    //         ASSERT(t0.data[i] == d0[i] + ((i/4) % 3 + 1), "Every element of t0 should be incremented correctly after adding t1 to it.");
            
    //     }
    // }

}


