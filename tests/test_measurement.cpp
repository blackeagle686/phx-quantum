// PHX-Quantum :: Measurement Unit Tests
#include "quantum/circuit.h"
#include "quantum/measurement.h"
#include "core/types.h"
#include "core/random.h"
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

TEST(deterministic_zero) {
    // |0> should always measure 0
    RNG::instance().seed(42);
    Matrix<Complex> state = Matrix<Complex>::column({C_ONE, C_ZERO});
    auto result = measurement::measure_qubit(state, 0, 1);
    ASSERT_TRUE(result.outcome == 0);
    ASSERT_TRUE(std::abs(result.probability - 1.0) < 1e-8);
}

TEST(deterministic_one) {
    // |1> should always measure 1
    RNG::instance().seed(42);
    Matrix<Complex> state = Matrix<Complex>::column({C_ZERO, C_ONE});
    auto result = measurement::measure_qubit(state, 0, 1);
    ASSERT_TRUE(result.outcome == 1);
    ASSERT_TRUE(std::abs(result.probability - 1.0) < 1e-8);
}

TEST(probabilities_correct) {
    Circuit c(2);
    c.h(0);
    c.cnot(0, 1);
    c.execute();

    auto probs = measurement::probabilities(c.state());
    // Bell state: P(|00>) = 0.5, P(|11>) = 0.5
    ASSERT_TRUE(std::abs(probs[0] - 0.5) < 1e-8);  // |00>
    ASSERT_TRUE(std::abs(probs[1]) < 1e-8);          // |01>
    ASSERT_TRUE(std::abs(probs[2]) < 1e-8);          // |10>
    ASSERT_TRUE(std::abs(probs[3] - 0.5) < 1e-8);   // |11>
}

TEST(bell_state_shots) {
    // Over many shots, Bell state should give ~50% |00>, ~50% |11>
    RNG::instance().seed(12345);
    Circuit c(2);
    c.h(0);
    c.cnot(0, 1);

    auto results = measurement::run_shots(c, 10000);

    // Should only have |00> and |11>
    for (const auto& [bits, count] : results) {
        ASSERT_TRUE(bits == "00" || bits == "11");
    }

    // Each should be roughly 50% (within 5%)
    double p00 = results["00"] / 10000.0;
    double p11 = results["11"] / 10000.0;
    ASSERT_TRUE(std::abs(p00 - 0.5) < 0.05);
    ASSERT_TRUE(std::abs(p11 - 0.5) < 0.05);
}

TEST(to_bitstring) {
    ASSERT_TRUE(measurement::to_bitstring(0, 2) == "00");
    ASSERT_TRUE(measurement::to_bitstring(1, 2) == "01");
    ASSERT_TRUE(measurement::to_bitstring(2, 2) == "10");
    ASSERT_TRUE(measurement::to_bitstring(3, 2) == "11");
    ASSERT_TRUE(measurement::to_bitstring(5, 3) == "101");
}

int main() {
    std::cout << "\n  === PHX-Quantum Measurement Tests ===\n\n";
    std::cout << "\n  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n\n";
    return tests_failed > 0 ? 1 : 0;
}
