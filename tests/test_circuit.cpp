// PHX-Quantum :: Circuit Unit Tests
#include "quantum/circuit.h"
#include "quantum/measurement.h"
#include "core/types.h"
#include <iostream>
#include <cmath>

using namespace phx;
using Mat = Matrix<Complex>;
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

TEST(initial_state_is_zero) {
    Circuit c(2);
    c.execute();
    // |00> = [1, 0, 0, 0]^T
    ASSERT_TRUE(approx_equal(c.state()(0,0), C_ONE));
    ASSERT_TRUE(approx_equal(c.state()(1,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(2,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(3,0), C_ZERO));
}

TEST(bell_state) {
    // H(0) -> CNOT(0,1) should produce (|00>+|11>)/sqrt(2)
    Circuit c(2);
    c.h(0);
    c.cnot(0, 1);
    c.execute();

    Complex expected_amp{SQRT2_INV, 0};
    ASSERT_TRUE(approx_equal(c.state()(0,0), expected_amp));  // |00>
    ASSERT_TRUE(approx_equal(c.state()(1,0), C_ZERO));        // |01>
    ASSERT_TRUE(approx_equal(c.state()(2,0), C_ZERO));        // |10>
    ASSERT_TRUE(approx_equal(c.state()(3,0), expected_amp));   // |11>
}

TEST(x_gate_on_qubit0) {
    Circuit c(2);
    c.x(0);
    c.execute();
    // X on q0: |00> -> |10> = [0, 0, 1, 0]^T
    ASSERT_TRUE(approx_equal(c.state()(0,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(1,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(2,0), C_ONE));
    ASSERT_TRUE(approx_equal(c.state()(3,0), C_ZERO));
}

TEST(x_gate_on_qubit1) {
    Circuit c(2);
    c.x(1);
    c.execute();
    // X on q1: |00> -> |01> = [0, 1, 0, 0]^T
    ASSERT_TRUE(approx_equal(c.state()(0,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(1,0), C_ONE));
    ASSERT_TRUE(approx_equal(c.state()(2,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(3,0), C_ZERO));
}

TEST(swap_gate) {
    Circuit c(2);
    c.x(0);        // |00> -> |10>
    c.swap(0, 1);  // |10> -> |01>
    c.execute();
    ASSERT_TRUE(approx_equal(c.state()(0,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(1,0), C_ONE));   // |01>
    ASSERT_TRUE(approx_equal(c.state()(2,0), C_ZERO));
    ASSERT_TRUE(approx_equal(c.state()(3,0), C_ZERO));
}

TEST(step_by_step_execution) {
    Circuit c(1);
    c.h(0);
    c.h(0);

    // Step 1: H|0> = |+>
    bool more = c.step();
    ASSERT_TRUE(more);
    ASSERT_TRUE(std::abs(std::norm(c.state()(0,0)) - 0.5) < 1e-8);

    // Step 2: H|+> = |0>
    more = c.step();
    ASSERT_TRUE(!more);  // No more steps
    ASSERT_TRUE(approx_equal(c.state()(0,0), C_ONE));
}

TEST(three_qubit_ghz) {
    // H(0) -> CNOT(0,1) -> CNOT(1,2) produces GHZ state
    // (|000> + |111>) / sqrt(2)
    Circuit c(3);
    c.h(0);
    c.cnot(0, 1);
    c.cnot(1, 2);
    c.execute();

    Complex expected{SQRT2_INV, 0};
    ASSERT_TRUE(approx_equal(c.state()(0,0), expected));  // |000>
    ASSERT_TRUE(approx_equal(c.state()(7,0), expected));  // |111>

    // All other amplitudes should be zero
    for (size_t i = 1; i < 7; ++i) {
        ASSERT_TRUE(approx_equal(c.state()(i,0), C_ZERO));
    }
}

int main() {
    std::cout << "\n  === PHX-Quantum Circuit Tests ===\n\n";
    std::cout << "\n  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n\n";
    return tests_failed > 0 ? 1 : 0;
}
