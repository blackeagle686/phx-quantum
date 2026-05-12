#pragma once
// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Quantum Circuit Builder & Executor
// ═══════════════════════════════════════════════════════════════════
//
//  A Circuit holds an ordered list of gate operations and a system
//  state vector. Gates are expanded to full system size using
//  Kronecker products before multiplication.
//
//  Usage:
//      Circuit c(2);          // 2-qubit system
//      c.h(0);                // Hadamard on qubit 0
//      c.cnot(0, 1);          // CNOT: control=0, target=1
//      c.execute();           // Run all ops
//      c.print_state(cout);   // Print final state
//
// ═══════════════════════════════════════════════════════════════════

#include "math/matrix.h"
#include "math/linalg.h"
#include "quantum/gates.h"
#include "core/types.h"
#include <vector>
#include <string>
#include <iostream>
#include <functional>

namespace phx {

/// Represents a single gate operation in a circuit
struct GateOp {
    std::string name;                 // Human-readable name
    Matrix<Complex> matrix;           // The gate matrix (may be 2×2 or 4×4)
    std::vector<size_t> targets;      // Qubit indices this gate acts on
    bool is_multi_qubit;              // true for CNOT, SWAP, etc.
};

class Circuit {
public:
    /// Create a circuit with `num_qubits` qubits, all initialized to |0⟩
    explicit Circuit(size_t num_qubits);

    /// Get the number of qubits
    size_t num_qubits() const;

    /// Get the full state vector dimension (2^n)
    size_t dim() const;

    // ─── Single-Qubit Gate API ────────────────────────────────────

    void i(size_t qubit);
    void h(size_t qubit);
    void x(size_t qubit);
    void y(size_t qubit);
    void z(size_t qubit);
    void s(size_t qubit);
    void t(size_t qubit);
    void rx(size_t qubit, double theta);
    void ry(size_t qubit, double theta);
    void rz(size_t qubit, double theta);

    // ─── Two-Qubit Gate API ──────────────────────────────────────

    void cnot(size_t control, size_t target);
    void swap(size_t q1, size_t q2);
    void cz(size_t control, size_t target);

    // ─── Custom Gate ─────────────────────────────────────────────

    void add_gate(const std::string& name,
                  const Matrix<Complex>& gate,
                  const std::vector<size_t>& targets);

    // ─── Execution ───────────────────────────────────────────────

    /// Execute all pending operations
    void execute();

    /// Execute one operation at a time (returns false when done)
    bool step();

    /// Reset the circuit (clear ops, reset state to |00...0⟩)
    void reset();

    /// Reset state only (keep ops for re-execution)
    void reset_state();

    // ─── State Access ────────────────────────────────────────────

    const Matrix<Complex>& state() const;
    void set_state(const Matrix<Complex>& s);

    /// Get the list of operations
    const std::vector<GateOp>& ops() const;

    /// Current step index (for step-by-step execution)
    size_t current_step() const;

    // ─── Display ─────────────────────────────────────────────────

    void print_state(std::ostream& os = std::cout) const;
    void print_circuit(std::ostream& os = std::cout) const;

private:
    size_t num_qubits_;
    size_t dim_;                  // 2^num_qubits
    Matrix<Complex> state_;       // Full system state vector (dim × 1)
    std::vector<GateOp> ops_;
    size_t step_index_ = 0;

    /// Build the full-system matrix for a single-qubit gate on target
    Matrix<Complex> expand_single_gate(const Matrix<Complex>& gate,
                                        size_t target) const;

    /// Build the full-system matrix for a two-qubit gate
    Matrix<Complex> expand_two_qubit_gate(const Matrix<Complex>& gate,
                                           size_t q1, size_t q2) const;

    /// Apply a full-system gate to the state vector
    void apply_to_state(const Matrix<Complex>& full_gate);

    /// Validate qubit index
    void check_qubit(size_t q, const char* gate_name) const;

    /// Initialize state to |00...0⟩
    Matrix<Complex> make_initial_state() const;
};

} // namespace phx
