// PHX-Quantum :: Gate Unit Tests
#include "quantum/gates.h"
#include "quantum/qubit.h"
#include "math/linalg.h"
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

// Gate is unitary: U†U = I
static void assert_unitary(const Mat& U, const char* name) {
    auto I = Mat::identity(U.rows());
    auto product = U.adjoint() * U;
    if (!product.approx_equals(I, 1e-8))
        throw std::runtime_error(std::string(name) + " is not unitary");
}

TEST(all_gates_unitary) {
    assert_unitary(gates::I(), "I");
    assert_unitary(gates::X(), "X");
    assert_unitary(gates::Y(), "Y");
    assert_unitary(gates::Z(), "Z");
    assert_unitary(gates::H(), "H");
    assert_unitary(gates::S(), "S");
    assert_unitary(gates::T(), "T");
    assert_unitary(gates::CNOT(), "CNOT");
    assert_unitary(gates::SWAP(), "SWAP");
    assert_unitary(gates::CZ(), "CZ");
}

TEST(X_flips_zero_to_one) {
    auto ket0 = Mat::column({C_ONE, C_ZERO});
    auto result = gates::apply(gates::X(), ket0);
    ASSERT_TRUE(approx_equal(result(0,0), C_ZERO));
    ASSERT_TRUE(approx_equal(result(1,0), C_ONE));
}

TEST(X_flips_one_to_zero) {
    auto ket1 = Mat::column({C_ZERO, C_ONE});
    auto result = gates::apply(gates::X(), ket1);
    ASSERT_TRUE(approx_equal(result(0,0), C_ONE));
    ASSERT_TRUE(approx_equal(result(1,0), C_ZERO));
}

TEST(H_creates_superposition) {
    auto ket0 = Mat::column({C_ONE, C_ZERO});
    auto result = gates::apply(gates::H(), ket0);
    // H|0> = (|0>+|1>)/sqrt(2)
    ASSERT_TRUE(approx_equal(result(0,0), Complex{SQRT2_INV, 0}));
    ASSERT_TRUE(approx_equal(result(1,0), Complex{SQRT2_INV, 0}));
}

TEST(HH_is_identity) {
    auto H = gates::H();
    auto HH = H * H;
    auto I = Mat::identity(2);
    ASSERT_TRUE(HH.approx_equals(I, 1e-8));
}

TEST(XX_is_identity) {
    auto X = gates::X();
    auto XX = X * X;
    auto I = Mat::identity(2);
    ASSERT_TRUE(XX.approx_equals(I, 1e-8));
}

TEST(Z_phase_flip) {
    // Z|+> = |->
    auto ket_plus = Mat::column({Complex{SQRT2_INV,0}, Complex{SQRT2_INV,0}});
    auto result = gates::apply(gates::Z(), ket_plus);
    // |-> = (|0>-|1>)/sqrt(2)
    ASSERT_TRUE(approx_equal(result(0,0), Complex{SQRT2_INV, 0}));
    ASSERT_TRUE(approx_equal(result(1,0), Complex{-SQRT2_INV, 0}));
}

TEST(rotation_gates) {
    // Rx(pi) should be equivalent to -iX (up to global phase)
    auto rx_pi = gates::Rx(PI);
    assert_unitary(rx_pi, "Rx(pi)");
    auto ry_pi = gates::Ry(PI);
    assert_unitary(ry_pi, "Ry(pi)");
    auto rz_pi = gates::Rz(PI);
    assert_unitary(rz_pi, "Rz(pi)");
}

int main() {
    std::cout << "\n  === PHX-Quantum Gate Tests ===\n\n";
    std::cout << "\n  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n\n";
    return tests_failed > 0 ? 1 : 0;
}
