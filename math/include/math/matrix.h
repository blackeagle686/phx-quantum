#pragma once
// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Matrix<T> — Core Linear Algebra Primitive
// ═══════════════════════════════════════════════════════════════════
//
//  Row-major dense matrix supporting complex arithmetic.
//  This is the backbone of the entire quantum simulator — every gate,
//  every state vector, every operation goes through this class.
//
// ═══════════════════════════════════════════════════════════════════

#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <complex>
#include <cassert>
#include <sstream>
#include <string>

#include "core/types.h"

namespace phx {

template<typename T>
class Matrix {
public:
    // ─── Construction ─────────────────────────────────────────────

    /// Create zero-initialized matrix
    Matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), data_(rows * cols, T{})
    {
        if (rows == 0 || cols == 0)
            throw std::invalid_argument("Matrix dimensions must be > 0");
    }

    /// Create from initializer list (row-major order)
    Matrix(size_t rows, size_t cols, std::initializer_list<T> data)
        : rows_(rows), cols_(cols), data_(data)
    {
        if (data_.size() != rows * cols)
            throw std::invalid_argument("Initializer list size mismatch");
    }

    /// Create from vector (row-major order)
    Matrix(size_t rows, size_t cols, std::vector<T> data)
        : rows_(rows), cols_(cols), data_(std::move(data))
    {
        if (data_.size() != rows_ * cols_)
            throw std::invalid_argument("Vector size mismatch");
    }

    /// Identity matrix
    static Matrix identity(size_t n) {
        Matrix m(n, n);
        for (size_t i = 0; i < n; ++i)
            m(i, i) = T{1};
        return m;
    }

    /// Zero matrix
    static Matrix zeros(size_t rows, size_t cols) {
        return Matrix(rows, cols);
    }

    /// Column vector from initializer list
    static Matrix column(std::initializer_list<T> data) {
        return Matrix(data.size(), 1, data);
    }

    // ─── Accessors ────────────────────────────────────────────────

    T& operator()(size_t r, size_t c) {
        assert(r < rows_ && c < cols_);
        return data_[r * cols_ + c];
    }

    const T& operator()(size_t r, size_t c) const {
        assert(r < rows_ && c < cols_);
        return data_[r * cols_ + c];
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    size_t size() const { return data_.size(); }

    const std::vector<T>& data() const { return data_; }
    std::vector<T>& data() { return data_; }

    // ─── Arithmetic ───────────────────────────────────────────────

    /// Matrix multiplication (O(n³))
    Matrix operator*(const Matrix& rhs) const {
        if (cols_ != rhs.rows_)
            throw std::invalid_argument(
                "Matrix multiply dimension mismatch: (" +
                std::to_string(rows_) + "x" + std::to_string(cols_) +
                ") * (" + std::to_string(rhs.rows_) + "x" +
                std::to_string(rhs.cols_) + ")");

        Matrix result(rows_, rhs.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t k = 0; k < cols_; ++k) {
                const T a_ik = data_[i * cols_ + k];
                for (size_t j = 0; j < rhs.cols_; ++j) {
                    result.data_[i * rhs.cols_ + j] += a_ik * rhs.data_[k * rhs.cols_ + j];
                }
            }
        }
        return result;
    }

    /// Element-wise addition
    Matrix operator+(const Matrix& rhs) const {
        check_dims(rhs, "addition");
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] + rhs.data_[i];
        return result;
    }

    /// Element-wise subtraction
    Matrix operator-(const Matrix& rhs) const {
        check_dims(rhs, "subtraction");
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] - rhs.data_[i];
        return result;
    }

    /// Scalar multiplication
    Matrix operator*(T scalar) const {
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] * scalar;
        return result;
    }

    /// Equality comparison (exact)
    bool operator==(const Matrix& rhs) const {
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_) return false;
        return data_ == rhs.data_;
    }

    bool operator!=(const Matrix& rhs) const {
        return !(*this == rhs);
    }

    // ─── Transformations ──────────────────────────────────────────

    /// Transpose
    Matrix transpose() const {
        Matrix result(cols_, rows_);
        for (size_t i = 0; i < rows_; ++i)
            for (size_t j = 0; j < cols_; ++j)
                result(j, i) = (*this)(i, j);
        return result;
    }

    /// Complex conjugate (element-wise)
    Matrix conjugate() const {
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = std::conj(data_[i]);
        return result;
    }

    /// Adjoint (conjugate transpose, a.k.a. dagger †)
    Matrix adjoint() const {
        return transpose().conjugate();
    }

    // ─── Approximate Comparison ───────────────────────────────────

    /// Check if two matrices are approximately equal
    bool approx_equals(const Matrix& rhs, double eps = EPSILON) const {
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_) return false;
        for (size_t i = 0; i < data_.size(); ++i) {
            if (std::abs(data_[i] - rhs.data_[i]) > eps)
                return false;
        }
        return true;
    }

    // ─── Display ──────────────────────────────────────────────────

    void print(std::ostream& os = std::cout) const {
        os << "[" << rows_ << "x" << cols_ << "]\n";
        for (size_t i = 0; i < rows_; ++i) {
            os << "  │ ";
            for (size_t j = 0; j < cols_; ++j) {
                const T& v = (*this)(i, j);
                os << std::setw(10) << std::setprecision(4) << v;
                if (j + 1 < cols_) os << "  ";
            }
            os << " │\n";
        }
    }

    std::string to_string() const {
        std::ostringstream oss;
        print(oss);
        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        m.print(os);
        return os;
    }

private:
    size_t rows_, cols_;
    std::vector<T> data_;

    void check_dims(const Matrix& rhs, const char* op) const {
        if (rows_ != rhs.rows_ || cols_ != rhs.cols_)
            throw std::invalid_argument(
                std::string("Dimension mismatch for ") + op);
    }
};

} // namespace phx
