// PHX-Quantum :: Tensor Unit Tests
#include "tensor/tensor.h"
#include "core/types.h"
#include <iostream>
#include <cmath>

using namespace phx;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    static void test_##name(); \
    struct Register_##name { Register_##name() { \
        std::cout << "  [RUN ] " << #name << std::endl; \
        try { test_##name(); tests_passed++; \
              std::cout << "  [PASS] " << #name << std::endl; \
        } catch (const std::exception& e) { tests_failed++; \
              std::cout << "  [FAIL] " << #name << ": " << e.what() << std::endl; \
        } \
    }} reg_##name; \
    static void test_##name()

#define ASSERT_TRUE(cond) \
    if (!(cond)) throw std::runtime_error("Assertion failed: " #cond)

TEST(construction) {
    Tensor<double> t({2, 3, 4});
    ASSERT_TRUE(t.ndim() == 3);
    ASSERT_TRUE(t.total_size() == 24);
    ASSERT_TRUE(t.shape()[0] == 2);
    ASSERT_TRUE(t.shape()[1] == 3);
    ASSERT_TRUE(t.shape()[2] == 4);
}

TEST(access_and_set) {
    Tensor<double> t({2, 3});
    t.at({0, 0}) = 1.0;
    t.at({1, 2}) = 5.0;
    ASSERT_TRUE(t.at({0, 0}) == 1.0);
    ASSERT_TRUE(t.at({1, 2}) == 5.0);
    ASSERT_TRUE(t.at({0, 1}) == 0.0);
}

TEST(reshape) {
    Tensor<int> t({2, 3}, {1, 2, 3, 4, 5, 6});
    auto r = t.reshape({3, 2});
    ASSERT_TRUE(r.shape()[0] == 3);
    ASSERT_TRUE(r.shape()[1] == 2);
    ASSERT_TRUE(r.total_size() == 6);
    ASSERT_TRUE(r.at({0, 0}) == 1);
    ASSERT_TRUE(r.at({2, 1}) == 6);
}

TEST(addition) {
    Tensor<double> a({2, 2}, {1, 2, 3, 4});
    Tensor<double> b({2, 2}, {5, 6, 7, 8});
    auto c = a + b;
    ASSERT_TRUE(c.at({0, 0}) == 6.0);
    ASSERT_TRUE(c.at({1, 1}) == 12.0);
}

TEST(scalar_multiply) {
    Tensor<double> t({2, 2}, {1, 2, 3, 4});
    auto r = t * 2.0;
    ASSERT_TRUE(r.at({0, 0}) == 2.0);
    ASSERT_TRUE(r.at({1, 1}) == 8.0);
}

TEST(hadamard_product) {
    Tensor<double> a({2, 2}, {1, 2, 3, 4});
    Tensor<double> b({2, 2}, {5, 6, 7, 8});
    auto c = a.hadamard(b);
    ASSERT_TRUE(c.at({0, 0}) == 5.0);
    ASSERT_TRUE(c.at({1, 1}) == 32.0);
}

TEST(fill) {
    Tensor<double> t({3, 3});
    t.fill(7.0);
    for (size_t i = 0; i < t.total_size(); ++i)
        ASSERT_TRUE(t.flat(i) == 7.0);
}

TEST(complex_tensor) {
    Tensor<Complex> t({2, 2}, {C_ONE, C_I, C_NEG, C_ZERO});
    ASSERT_TRUE(t.at({0, 1}) == C_I);
    ASSERT_TRUE(t.at({1, 0}) == C_NEG);
}

int main() {
    std::cout << "\n  === PHX-Quantum Tensor Tests ===\n\n";
    std::cout << "\n  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n\n";
    return tests_failed > 0 ? 1 : 0;
}
