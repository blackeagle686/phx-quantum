#pragma once
// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Linear Algebra Utilities
// ═══════════════════════════════════════════════════════════════════
//
//  Standalone functions that operate on Matrix<T>:
//    - Kronecker (tensor) product
//    - Vector norm / normalize
//    - Inner product
//
// ═══════════════════════════════════════════════════════════════════

#include "math/matrix.h"
#include <cmath>
#include <complex>

namespace phx::linalg {

// ─── Kronecker Product ────────────────────────────────────────────
//
//  Given A (m×n) and B (p×q), returns C (mp × nq) where:
//      C[i*p+k][j*q+l] = A[i][j] * B[k][l]
//
//  This is THE critical operation for building multi-qubit systems.
//  Example: kron(|0⟩, |1⟩) = |01⟩
//           kron(H, I) applies H to first qubit in a 2-qubit system
//
template<typename T>
Matrix<T> kronecker(const Matrix<T>& A, const Matrix<T>& B) {
    const size_t m = A.rows(), n = A.cols();
    const size_t p = B.rows(), q = B.cols();

    Matrix<T> result(m * p, n * q);

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            const T a_ij = A(i, j);
            for (size_t k = 0; k < p; ++k) {
                for (size_t l = 0; l < q; ++l) {
                    result(i * p + k, j * q + l) = a_ij * B(k, l);
                }
            }
        }
    }

    return result;
}

// ─── Vector Norm (L2) ─────────────────────────────────────────────
//
//  For a column vector v, returns √(Σ|vᵢ|²)
//  Works with complex entries: |vᵢ|² = vᵢ * conj(vᵢ)
//
template<typename T>
double norm(const Matrix<T>& vec) {
    double sum = 0.0;
    for (size_t i = 0; i < vec.size(); ++i) {
        sum += std::norm(vec.data()[i]);  // std::norm gives |z|²
    }
    return std::sqrt(sum);
}

// ─── Normalize ────────────────────────────────────────────────────
//
//  Divides every element by the vector's norm so that ||v|| = 1.
//  This is essential after measurement collapse.
//
template<typename T>
void normalize(Matrix<T>& vec) {
    double n = norm(vec);
    if (n < EPSILON)
        throw std::runtime_error("Cannot normalize zero vector");
    T inv_n = T{1.0 / n};
    for (size_t i = 0; i < vec.size(); ++i) {
        vec.data()[i] *= inv_n;
    }
}

// ─── Inner Product ⟨a|b⟩ ─────────────────────────────────────────
//
//  For column vectors a and b: ⟨a|b⟩ = Σ conj(aᵢ) * bᵢ
//  This is the quantum mechanical bracket notation.
//
template<typename T>
T inner_product(const Matrix<T>& a, const Matrix<T>& b) {
    if (a.size() != b.size())
        throw std::invalid_argument("Inner product: size mismatch");

    T result{};
    for (size_t i = 0; i < a.size(); ++i) {
        result += std::conj(a.data()[i]) * b.data()[i];
    }
    return result;
}

// ─── Outer Product |a⟩⟨b| ────────────────────────────────────────
//
//  Creates a matrix from two column vectors.
//
template<typename T>
Matrix<T> outer_product(const Matrix<T>& a, const Matrix<T>& b) {
    Matrix<T> result(a.rows(), b.rows());
    for (size_t i = 0; i < a.rows(); ++i) {
        for (size_t j = 0; j < b.rows(); ++j) {
            result(i, j) = a(i, 0) * std::conj(b(j, 0));
        }
    }
    return result;
}

} // namespace phx::linalg
