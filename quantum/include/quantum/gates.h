#pragma once
// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Quantum Gate Definitions
// ═══════════════════════════════════════════════════════════════════
//
//  Each gate is a unitary matrix. Single-qubit gates are 2×2,
//  two-qubit gates are 4×4. All returned as Matrix<Complex>.
//
//  Gate application: |ψ'⟩ = U|ψ⟩  (matrix × state vector)
//
// ═══════════════════════════════════════════════════════════════════

#include "math/matrix.h"
#include "core/types.h"

namespace phx::gates {

// ─── Single-Qubit Gates (2×2) ─────────────────────────────────────

/// Identity gate — does nothing
///  ┌     ┐
///  │ 1 0 │
///  │ 0 1 │
///  └     ┘
Matrix<Complex> I();

/// Pauli-X gate (NOT / bit-flip)
///  ┌     ┐
///  │ 0 1 │
///  │ 1 0 │
///  └     ┘
Matrix<Complex> X();

/// Pauli-Y gate
///  ┌       ┐
///  │ 0  -i │
///  │ i   0 │
///  └       ┘
Matrix<Complex> Y();

/// Pauli-Z gate (phase-flip)
///  ┌      ┐
///  │ 1  0 │
///  │ 0 -1 │
///  └      ┘
Matrix<Complex> Z();

/// Hadamard gate — creates superposition
///  1/√2 ┌      ┐
///       │ 1  1 │
///       │ 1 -1 │
///       └      ┘
Matrix<Complex> H();

/// S gate (Phase gate, √Z)
///  ┌     ┐
///  │ 1 0 │
///  │ 0 i │
///  └     ┘
Matrix<Complex> S();

/// T gate (π/8 gate, √S)
///  ┌            ┐
///  │ 1     0    │
///  │ 0  e^iπ/4 │
///  └            ┘
Matrix<Complex> T();

/// Rotation around X-axis by angle θ
Matrix<Complex> Rx(double theta);

/// Rotation around Y-axis by angle θ
Matrix<Complex> Ry(double theta);

/// Rotation around Z-axis by angle θ
Matrix<Complex> Rz(double theta);

// ─── Two-Qubit Gates (4×4) ───────────────────────────────────────

/// CNOT (Controlled-NOT) gate
///  ┌         ┐
///  │ 1 0 0 0 │    |00⟩ → |00⟩
///  │ 0 1 0 0 │    |01⟩ → |01⟩
///  │ 0 0 0 1 │    |10⟩ → |11⟩
///  │ 0 0 1 0 │    |11⟩ → |10⟩
///  └         ┘
Matrix<Complex> CNOT();

/// SWAP gate
///  ┌         ┐
///  │ 1 0 0 0 │    |00⟩ → |00⟩
///  │ 0 0 1 0 │    |01⟩ → |10⟩
///  │ 0 1 0 0 │    |10⟩ → |01⟩
///  │ 0 0 0 1 │    |11⟩ → |11⟩
///  └         ┘
Matrix<Complex> SWAP();

/// Controlled-Z gate
///  ┌          ┐
///  │ 1  0 0  0│
///  │ 0  1 0  0│
///  │ 0  0 1  0│
///  │ 0  0 0 -1│
///  └          ┘
Matrix<Complex> CZ();

// ─── Gate Application ─────────────────────────────────────────────

/// Apply a gate matrix to a state vector: |ψ'⟩ = U|ψ⟩
Matrix<Complex> apply(const Matrix<Complex>& gate,
                      const Matrix<Complex>& state);

} // namespace phx::gates
