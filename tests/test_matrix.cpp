// PHX-Quantum :: Matrix Unit Tests
#include "math/matrix.h"
#include "math/linalg.h"
#include "core/types.h"
#include <iostream>
#include <cassert>
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

// Use a function instead of macro to avoid MSVC comma-in-braces issues
static void assert_approx(Complex a, Complex b, const char* msg) {
    if (!approx_equal(a, b, 1e-8))
        throw std::runtime_error(std::string("Approx failed: ") + msg);
}

// ─── Tests ──────────────────────────────────────────────────────

TEST(identity_2x2) {
    auto I = Mat::identity(2);
    assert_approx(I(0,0), C_ONE, "I(0,0)");
    assert_approx(I(0,1), C_ZERO, "I(0,1)");
    assert_approx(I(1,0), C_ZERO, "I(1,0)");
    assert_approx(I(1,1), C_ONE, "I(1,1)");
}

TEST(multiply_identity) {
    Mat A(2, 2, {C_ONE, Complex{2,0}, Complex{3,0}, Complex{4,0}});
    auto I = Mat::identity(2);
    auto R = A * I;
    ASSERT_TRUE(R.approx_equals(A));
}

TEST(multiply_matrices) {
    Mat A(2, 2, {C_ONE, Complex{2,0}, Complex{3,0}, Complex{4,0}});
    Mat B(2, 2, {Complex{5,0}, Complex{6,0}, Complex{7,0}, Complex{8,0}});
    auto C = A * B;
    assert_approx(C(0,0), Complex(19,0), "C(0,0)=19");
    assert_approx(C(0,1), Complex(22,0), "C(0,1)=22");
    assert_approx(C(1,0), Complex(43,0), "C(1,0)=43");
    assert_approx(C(1,1), Complex(50,0), "C(1,1)=50");
}

TEST(matrix_addition) {
    Mat A(2, 2, {C_ONE, C_ZERO, C_ZERO, C_ONE});
    Mat B(2, 2, {C_ONE, C_ONE, C_ONE, C_ONE});
    auto C = A + B;
    assert_approx(C(0,0), Complex(2,0), "C(0,0)=2");
    assert_approx(C(0,1), C_ONE, "C(0,1)=1");
    assert_approx(C(1,0), C_ONE, "C(1,0)=1");
    assert_approx(C(1,1), Complex(2,0), "C(1,1)=2");
}

TEST(transpose) {
    Mat A(2, 3, {C_ONE, Complex(2,0), Complex(3,0),
                 Complex(4,0), Complex(5,0), Complex(6,0)});
    auto T = A.transpose();
    ASSERT_TRUE(T.rows() == 3 && T.cols() == 2);
    assert_approx(T(0,0), C_ONE, "T(0,0)");
    assert_approx(T(1,0), Complex(2,0), "T(1,0)");
    assert_approx(T(2,1), Complex(6,0), "T(2,1)");
}

TEST(conjugate) {
    Mat A(2, 1, {Complex(1,2), Complex(3,-4)});
    auto C = A.conjugate();
    assert_approx(C(0,0), Complex(1,-2), "conj(1+2i)");
    assert_approx(C(1,0), Complex(3,4), "conj(3-4i)");
}

TEST(adjoint) {
    Mat A(2, 2, {Complex(1,1), Complex(2,-1),
                 Complex(3,0), Complex(0,4)});
    auto D = A.adjoint();
    assert_approx(D(0,0), Complex(1,-1), "D(0,0)");
    assert_approx(D(0,1), Complex(3,0), "D(0,1)");
    assert_approx(D(1,0), Complex(2,1), "D(1,0)");
    assert_approx(D(1,1), Complex(0,-4), "D(1,1)");
}

TEST(scalar_multiply) {
    Mat A(2, 1, {C_ONE, Complex(2,0)});
    auto B = A * Complex(3,0);
    assert_approx(B(0,0), Complex(3,0), "B(0,0)");
    assert_approx(B(1,0), Complex(6,0), "B(1,0)");
}

TEST(kronecker_identity) {
    auto I2 = Mat::identity(2);
    auto I4 = linalg::kronecker(I2, I2);
    ASSERT_TRUE(I4.rows() == 4 && I4.cols() == 4);
    auto expected = Mat::identity(4);
    ASSERT_TRUE(I4.approx_equals(expected));
}

TEST(kronecker_basis_states) {
    // |0> ⊗ |1> = |01> = [0, 1, 0, 0]^T
    Mat ket0 = Mat::column({C_ONE, C_ZERO});
    Mat ket1 = Mat::column({C_ZERO, C_ONE});
    auto ket01 = linalg::kronecker(ket0, ket1);
    ASSERT_TRUE(ket01.rows() == 4 && ket01.cols() == 1);
    assert_approx(ket01(0,0), C_ZERO, "ket01(0)");
    assert_approx(ket01(1,0), C_ONE, "ket01(1)");
    assert_approx(ket01(2,0), C_ZERO, "ket01(2)");
    assert_approx(ket01(3,0), C_ZERO, "ket01(3)");
}

TEST(vector_norm) {
    Mat v = Mat::column({Complex(3,0), Complex(4,0)});
    double n = linalg::norm(v);
    ASSERT_TRUE(std::abs(n - 5.0) < 1e-10);
}

TEST(normalize_vector) {
    Mat v = Mat::column({Complex(3,0), Complex(4,0)});
    linalg::normalize(v);
    double n = linalg::norm(v);
    ASSERT_TRUE(std::abs(n - 1.0) < 1e-10);
}

TEST(inner_product) {
    Mat a = Mat::column({C_ONE, C_I});
    Mat b = Mat::column({C_ONE, C_ONE});
    auto ip = linalg::inner_product(a, b);
    // <a|b> = conj(1)*1 + conj(i)*1 = 1 + (-i) = 1-i
    assert_approx(ip, Complex(1, -1), "inner_product");
}

int main() {
    std::cout << "\n  === PHX-Quantum Matrix Tests ===\n\n";
    // Tests auto-register via static construction
    std::cout << "\n  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n\n";
    return tests_failed > 0 ? 1 : 0;
}
