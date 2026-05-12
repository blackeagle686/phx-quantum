#pragma once
// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Qubit State Vector
// ═══════════════════════════════════════════════════════════════════
//
//  A qubit is a 2×1 column vector |ψ⟩ = α|0⟩ + β|1⟩
//  where |α|² + |β|² = 1
//
// ═══════════════════════════════════════════════════════════════════

#include "math/matrix.h"
#include "math/linalg.h"
#include <iostream>

namespace phx {

class Qubit {
public:
    /// Default: |0⟩ state
    Qubit();

    /// Custom state (will be normalized)
    Qubit(Complex alpha, Complex beta);

    /// Named constructors
    static Qubit zero();   // |0⟩ = [1, 0]ᵀ
    static Qubit one();    // |1⟩ = [0, 1]ᵀ
    static Qubit plus();   // |+⟩ = (|0⟩ + |1⟩)/√2
    static Qubit minus();  // |−⟩ = (|0⟩ − |1⟩)/√2

    /// Access the raw state vector (2×1 matrix)
    const Matrix<Complex>& state() const;
    void set_state(const Matrix<Complex>& s);

    /// Amplitudes
    Complex alpha() const;  // ⟨0|ψ⟩
    Complex beta() const;   // ⟨1|ψ⟩

    /// Probabilities
    double prob_zero() const;   // |α|²
    double prob_one() const;    // |β|²

    /// Normalize the state vector
    void normalize();

    /// Pretty print
    void print(std::ostream& os = std::cout) const;

    friend std::ostream& operator<<(std::ostream& os, const Qubit& q) {
        q.print(os);
        return os;
    }

private:
    Matrix<Complex> state_;  // 2×1 column vector
};

} // namespace phx
