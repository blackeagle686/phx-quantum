// PHX-Quantum :: Qubit Unit Tests
#include "quantum/qubit.h"
#include "core/types.h"
#include <iostream>
#include <cassert>
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

TEST(default_is_zero) {
    Qubit q;
    ASSERT_TRUE(approx_equal(q.alpha(), C_ONE));
    ASSERT_TRUE(approx_equal(q.beta(), C_ZERO));
}

TEST(ket_one) {
    auto q = Qubit::one();
    ASSERT_TRUE(approx_equal(q.alpha(), C_ZERO));
    ASSERT_TRUE(approx_equal(q.beta(), C_ONE));
}

TEST(ket_plus) {
    auto q = Qubit::plus();
    ASSERT_TRUE(std::abs(q.prob_zero() - 0.5) < 1e-8);
    ASSERT_TRUE(std::abs(q.prob_one() - 0.5) < 1e-8);
}

TEST(probabilities_sum_to_one) {
    Qubit q(Complex{0.6, 0}, Complex{0.8, 0});
    double sum = q.prob_zero() + q.prob_one();
    ASSERT_TRUE(std::abs(sum - 1.0) < 1e-8);
}

TEST(normalization) {
    Qubit q(Complex{3, 0}, Complex{4, 0});
    double sum = q.prob_zero() + q.prob_one();
    ASSERT_TRUE(std::abs(sum - 1.0) < 1e-8);
}

TEST(complex_amplitudes) {
    Qubit q(Complex{1, 1}, Complex{1, -1});
    double sum = q.prob_zero() + q.prob_one();
    ASSERT_TRUE(std::abs(sum - 1.0) < 1e-8);
}

int main() {
    std::cout << "\n  === PHX-Quantum Qubit Tests ===\n\n";
    std::cout << "\n  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n\n";
    return tests_failed > 0 ? 1 : 0;
}
