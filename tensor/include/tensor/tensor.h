#pragma once
// PHX-Quantum :: N-Dimensional Tensor Core
#include <vector>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <functional>
#include <sstream>
#include "core/types.h"

namespace phx {

template<typename T>
class Tensor {
public:
    Tensor(std::vector<size_t> shape)
        : shape_(std::move(shape))
        , data_(total_elements(), T{})
    {}

    Tensor(std::vector<size_t> shape, std::vector<T> data)
        : shape_(std::move(shape))
        , data_(std::move(data))
    {
        if (data_.size() != total_elements())
            throw std::invalid_argument("Tensor data size != shape product");
    }

    // ─── Access ──────────────────────────────────────────────
    T& at(const std::vector<size_t>& indices) {
        return data_[flat_index(indices)];
    }

    const T& at(const std::vector<size_t>& indices) const {
        return data_[flat_index(indices)];
    }

    T& flat(size_t i) { return data_[i]; }
    const T& flat(size_t i) const { return data_[i]; }

    const std::vector<size_t>& shape() const { return shape_; }
    size_t ndim() const { return shape_.size(); }
    size_t total_size() const { return data_.size(); }
    const std::vector<T>& data() const { return data_; }

    // ─── Operations ──────────────────────────────────────────
    Tensor reshape(std::vector<size_t> new_shape) const {
        size_t new_total = 1;
        for (auto s : new_shape) new_total *= s;
        if (new_total != data_.size())
            throw std::invalid_argument("Reshape: total size mismatch");
        return Tensor(std::move(new_shape), data_);
    }

    Tensor operator+(const Tensor& rhs) const {
        check_shape(rhs, "addition");
        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] + rhs.data_[i];
        return result;
    }

    Tensor operator-(const Tensor& rhs) const {
        check_shape(rhs, "subtraction");
        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] - rhs.data_[i];
        return result;
    }

    Tensor operator*(T scalar) const {
        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] * scalar;
        return result;
    }

    // Element-wise multiply
    Tensor hadamard(const Tensor& rhs) const {
        check_shape(rhs, "element-wise multiply");
        Tensor result(shape_);
        for (size_t i = 0; i < data_.size(); ++i)
            result.data_[i] = data_[i] * rhs.data_[i];
        return result;
    }

    void fill(T value) {
        std::fill(data_.begin(), data_.end(), value);
    }

    // ─── Utility ─────────────────────────────────────────────
    void print(std::ostream& os = std::cout) const {
        os << "Tensor[";
        for (size_t i = 0; i < shape_.size(); ++i) {
            if (i > 0) os << "x";
            os << shape_[i];
        }
        os << "] = { ";
        size_t limit = std::min(data_.size(), size_t(16));
        for (size_t i = 0; i < limit; ++i) {
            os << data_[i];
            if (i + 1 < limit) os << ", ";
        }
        if (data_.size() > 16) os << ", ...";
        os << " }\n";
    }

    std::string to_string() const {
        std::ostringstream oss;
        print(oss);
        return oss.str();
    }

private:
    std::vector<size_t> shape_;
    std::vector<T> data_;

    size_t total_elements() const {
        if (shape_.empty()) return 0;
        size_t total = 1;
        for (auto s : shape_) total *= s;
        return total;
    }

    size_t flat_index(const std::vector<size_t>& indices) const {
        if (indices.size() != shape_.size())
            throw std::invalid_argument("Index dimensionality mismatch");
        size_t idx = 0;
        size_t stride = 1;
        for (int d = static_cast<int>(shape_.size()) - 1; d >= 0; --d) {
            if (indices[d] >= shape_[d])
                throw std::out_of_range("Tensor index out of range");
            idx += indices[d] * stride;
            stride *= shape_[d];
        }
        return idx;
    }

    void check_shape(const Tensor& rhs, const char* op) const {
        if (shape_ != rhs.shape_)
            throw std::invalid_argument(
                std::string("Shape mismatch for ") + op);
    }
};

} // namespace phx
