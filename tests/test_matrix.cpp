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

#define ASSERT_APPROX(a, b) \
    if (!approx_equal(a, b, 1e-8)) throw std::runtime_error( \
        "Approx assertion failed: " #a " != " #b)

// ─── Tests ──────────────────────────────────────────────────────

TEST(identity_2x2) {
    auto I = Mat::identity(2);
    ASSERT_APPROX(I(0,0), C_ONE);
    ASSERT_APPROX(I(0,1), C_ZERO);
    ASSERT_APPROX(I(1,0), C_ZERO);
    ASSERT_APPROX(I(1,1), C_ONE);
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
    // [1,2]*[5,6] = [1*5+2*7, 1*6+2*8] = [19, 22]
    // [3,4] [7,8]   [3*5+4*7, 3*6+4*8]   [43, 50]
    ASSERT_APPROX(C(0,0), Complex{19,0});
    ASSERT_APPROX(C(0,1), Complex{22,0});
    ASSERT_APPROX(C(1,0), Complex{43,0});
    ASSERT_APPROX(C(1,1), Complex{50,0});
}

TEST(matrix_addition) {
    Mat A(2, 2, {C_ONE, C_ZERO, C_ZERO, C_ONE});
    Mat B(2, 2, {C_ONE, C_ONE, C_ONE, C_ONE});
    auto C = A + B;
    ASSERT_APPROX(C(0,0), Complex{2,0});
    ASSERT_APPROX(C(0,1), C_ONE);
    ASSERT_APPROX(C(1,0), C_ONE);
    ASSERT_APPROX(C(1,1), Complex{2,0});
}

TEST(transpose) {
    Mat A(2, 3, {C_ONE, Complex{2,0}, Complex{3,0},
                 Complex{4,0}, Complex{5,0}, Complex{6,0}});
    auto T = A.transpose();
    ASSERT_TRUE(T.rows() == 3 && T.cols() == 2);
    ASSERT_APPROX(T(0,0), C_ONE);
    ASSERT_APPROX(T(1,0), Complex{2,0});
    ASSERT_APPROX(T(2,1), Complex{6,0});
}

TEST(conjugate) {
    Mat A(2, 1, {Complex{1,2}, Complex{3,-4}});
    auto C = A.conjugate();
    ASSERT_APPROX(C(0,0), Complex{1,-2});
    ASSERT_APPROX(C(1,0), Complex{3,4});
}

TEST(adjoint) {
    Mat A(2, 2, {Complex{1,1}, Complex{2,-1},
                 Complex{3,0}, Complex{0,4}});
    auto D = A.adjoint();
    // adjoint = conjugate transpose
    ASSERT_APPROX(D(0,0), Complex{1,-1});
    ASSERT_APPROX(D(0,1), Complex{3,0});
    ASSERT_APPROX(D(1,0), Complex{2,1});
    ASSERT_APPROX(D(1,1), Complex{0,-4});
}

TEST(scalar_multiply) {
    Mat A(2, 1, {C_ONE, Complex{2,0}});
    auto B = A * Complex{3,0};
    ASSERT_APPROX(B(0,0), Complex{3,0});
    ASSERT_APPROX(B(1,0), Complex{6,0});
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
    ASSERT_APPROX(ket01(0,0), C_ZERO);
    ASSERT_APPROX(ket01(1,0), C_ONE);
    ASSERT_APPROX(ket01(2,0), C_ZERO);
    ASSERT_APPROX(ket01(3,0), C_ZERO);
}

TEST(vector_norm) {
    Mat v = Mat::column({Complex{3,0}, Complex{4,0}});
    double n = linalg::norm(v);
    ASSERT_TRUE(std::abs(n - 5.0) < 1e-10);
}

TEST(normalize_vector) {
    Mat v = Mat::column({Complex{3,0}, Complex{4,0}});
    linalg::normalize(v);
    double n = linalg::norm(v);
    ASSERT_TRUE(std::abs(n - 1.0) < 1e-10);
}

TEST(inner_product) {
    Mat a = Mat::column({C_ONE, C_I});
    Mat b = Mat::column({C_ONE, C_ONE});
    auto ip = linalg::inner_product(a, b);
    // <a|b> = conj(1)*1 + conj(i)*1 = 1 + (-i) = 1-i
    ASSERT_APPROX(ip, Complex{1, -1});
}

int main() {
    std::cout << "\n  === PHX-Quantum Matrix Tests ===\n\n";
    // Tests auto-register via static construction
    std::cout << "\n  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n\n";
    return tests_failed > 0 ? 1 : 0;
}
