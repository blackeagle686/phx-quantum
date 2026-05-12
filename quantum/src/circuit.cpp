// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Circuit Implementation
// ═══════════════════════════════════════════════════════════════════
//
//  The key insight: to apply a 2×2 gate to qubit `i` in an n-qubit
//  system, we build the full 2ⁿ × 2ⁿ matrix using Kronecker products:
//
//    full = I ⊗ I ⊗ ... ⊗ Gate ⊗ ... ⊗ I
//                          ↑
//                       position i
//
//  For two-qubit gates (like CNOT) on adjacent qubits, we use:
//    full = I ⊗ ... ⊗ CNOT ⊗ ... ⊗ I
//
//  For non-adjacent qubits, we use SWAP chains to bring them
//  together, apply the gate, then SWAP back.
//
// ═══════════════════════════════════════════════════════════════════

#include "quantum/circuit.h"
#include <bitset>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace phx {

// ─── Construction ────────────────────────────────────────────────

Circuit::Circuit(size_t num_qubits)
    : num_qubits_(num_qubits)
    , dim_(1ULL << num_qubits)
    , state_(make_initial_state())
{
    if (num_qubits == 0 || num_qubits > 16)
        throw std::invalid_argument(
            "Circuit supports 1-16 qubits (got " +
            std::to_string(num_qubits) + ")");
}

size_t Circuit::num_qubits() const { return num_qubits_; }
size_t Circuit::dim() const { return dim_; }

Matrix<Complex> Circuit::make_initial_state() const {
    // |00...0⟩ = [1, 0, 0, ..., 0]ᵀ
    Matrix<Complex> s(dim_, 1);
    s(0, 0) = C_ONE;
    return s;
}

// ─── Single-Qubit Gates ─────────────────────────────────────────

void Circuit::i(size_t qubit) {
    check_qubit(qubit, "I");
    ops_.push_back({"I", gates::I(), {qubit}, false});
}

void Circuit::h(size_t qubit) {
    check_qubit(qubit, "H");
    ops_.push_back({"H", gates::H(), {qubit}, false});
}

void Circuit::x(size_t qubit) {
    check_qubit(qubit, "X");
    ops_.push_back({"X", gates::X(), {qubit}, false});
}

void Circuit::y(size_t qubit) {
    check_qubit(qubit, "Y");
    ops_.push_back({"Y", gates::Y(), {qubit}, false});
}

void Circuit::z(size_t qubit) {
    check_qubit(qubit, "Z");
    ops_.push_back({"Z", gates::Z(), {qubit}, false});
}

void Circuit::s(size_t qubit) {
    check_qubit(qubit, "S");
    ops_.push_back({"S", gates::S(), {qubit}, false});
}

void Circuit::t(size_t qubit) {
    check_qubit(qubit, "T");
    ops_.push_back({"T", gates::T(), {qubit}, false});
}

void Circuit::rx(size_t qubit, double theta) {
    check_qubit(qubit, "Rx");
    ops_.push_back({"Rx(" + std::to_string(theta) + ")",
                    gates::Rx(theta), {qubit}, false});
}

void Circuit::ry(size_t qubit, double theta) {
    check_qubit(qubit, "Ry");
    ops_.push_back({"Ry(" + std::to_string(theta) + ")",
                    gates::Ry(theta), {qubit}, false});
}

void Circuit::rz(size_t qubit, double theta) {
    check_qubit(qubit, "Rz");
    ops_.push_back({"Rz(" + std::to_string(theta) + ")",
                    gates::Rz(theta), {qubit}, false});
}

// ─── Two-Qubit Gates ────────────────────────────────────────────

void Circuit::cnot(size_t control, size_t target) {
    check_qubit(control, "CNOT");
    check_qubit(target, "CNOT");
    if (control == target)
        throw std::invalid_argument("CNOT: control and target must differ");
    ops_.push_back({"CNOT", gates::CNOT(), {control, target}, true});
}

void Circuit::swap(size_t q1, size_t q2) {
    check_qubit(q1, "SWAP");
    check_qubit(q2, "SWAP");
    if (q1 == q2)
        throw std::invalid_argument("SWAP: qubits must differ");
    ops_.push_back({"SWAP", gates::SWAP(), {q1, q2}, true});
}

void Circuit::cz(size_t control, size_t target) {
    check_qubit(control, "CZ");
    check_qubit(target, "CZ");
    if (control == target)
        throw std::invalid_argument("CZ: control and target must differ");
    ops_.push_back({"CZ", gates::CZ(), {control, target}, true});
}

// ─── Custom Gate ─────────────────────────────────────────────────

void Circuit::add_gate(const std::string& name,
                       const Matrix<Complex>& gate,
                       const std::vector<size_t>& targets) {
    for (auto q : targets) check_qubit(q, name.c_str());
    bool multi = targets.size() > 1;
    ops_.push_back({name, gate, targets, multi});
}

// ─── Execution ───────────────────────────────────────────────────

void Circuit::execute() {
    // Reset state to |00...0⟩ and replay all operations
    state_ = make_initial_state();
    step_index_ = 0;

    for (const auto& op : ops_) {
        Matrix<Complex> full_gate(dim_, dim_);

        if (!op.is_multi_qubit) {
            // Single-qubit gate: expand with Kronecker
            full_gate = expand_single_gate(op.matrix, op.targets[0]);
        } else {
            // Two-qubit gate: expand to full system
            full_gate = expand_two_qubit_gate(op.matrix,
                                               op.targets[0],
                                               op.targets[1]);
        }

        apply_to_state(full_gate);
        step_index_++;
    }
}

bool Circuit::step() {
    if (step_index_ >= ops_.size()) return false;

    if (step_index_ == 0) {
        state_ = make_initial_state();
    }

    const auto& op = ops_[step_index_];
    Matrix<Complex> full_gate(dim_, dim_);

    if (!op.is_multi_qubit) {
        full_gate = expand_single_gate(op.matrix, op.targets[0]);
    } else {
        full_gate = expand_two_qubit_gate(op.matrix,
                                           op.targets[0],
                                           op.targets[1]);
    }

    apply_to_state(full_gate);
    step_index_++;
    return true;
}

void Circuit::reset() {
    ops_.clear();
    state_ = make_initial_state();
    step_index_ = 0;
}

void Circuit::reset_state() {
    state_ = make_initial_state();
    step_index_ = 0;
}

// ─── State Access ────────────────────────────────────────────────

const Matrix<Complex>& Circuit::state() const { return state_; }

void Circuit::set_state(const Matrix<Complex>& s) {
    if (s.rows() != dim_ || s.cols() != 1)
        throw std::invalid_argument("State vector dimension mismatch");
    state_ = s;
}

const std::vector<GateOp>& Circuit::ops() const { return ops_; }
size_t Circuit::current_step() const { return step_index_; }

// ─── Display ─────────────────────────────────────────────────────

void Circuit::print_state(std::ostream& os) const {
    os << "State vector (" << num_qubits_ << " qubits, "
       << dim_ << " amplitudes):\n";

    for (size_t i = 0; i < dim_; ++i) {
        Complex amp = state_(i, 0);
        double prob = std::norm(amp);

        if (prob < EPSILON) continue;  // Skip zero amplitudes

        // Print basis state in binary
        os << "  |";
        for (int b = static_cast<int>(num_qubits_) - 1; b >= 0; --b) {
            os << ((i >> b) & 1);
        }
        os << "⟩  ";

        // Print amplitude
        os << std::setprecision(6) << std::fixed;
        os << "amp=(" << amp.real();
        if (std::abs(amp.imag()) > EPSILON) {
            os << (amp.imag() >= 0 ? "+" : "") << amp.imag() << "i";
        }
        os << ")";

        // Print probability
        os << "  P=" << std::setprecision(4) << (prob * 100.0) << "%\n";
    }
}

void Circuit::print_circuit(std::ostream& os) const {
    os << "Circuit (" << num_qubits_ << " qubits, "
       << ops_.size() << " gates):\n";

    // Simple text representation
    for (size_t i = 0; i < ops_.size(); ++i) {
        const auto& op = ops_[i];
        os << "  [" << i << "] " << op.name << "(";
        for (size_t j = 0; j < op.targets.size(); ++j) {
            if (j > 0) os << ", ";
            os << "q" << op.targets[j];
        }
        os << ")\n";
    }
}

// ─── Private Helpers ─────────────────────────────────────────────

Matrix<Complex> Circuit::expand_single_gate(
    const Matrix<Complex>& gate, size_t target) const
{
    // Build: I ⊗ ... ⊗ Gate ⊗ ... ⊗ I
    // Start with a 1×1 identity, then Kronecker each position
    Matrix<Complex> result = Matrix<Complex>::identity(1);

    for (size_t q = 0; q < num_qubits_; ++q) {
        if (q == target) {
            result = linalg::kronecker(result, gate);
        } else {
            result = linalg::kronecker(result, gates::I());
        }
    }

    return result;
}

Matrix<Complex> Circuit::expand_two_qubit_gate(
    const Matrix<Complex>& gate, size_t q1, size_t q2) const
{
    // For two-qubit gates, we need to handle arbitrary qubit positions.
    // The gate matrix is defined for a standard ordering where
    // the first qubit listed is higher-order.
    //
    // We build the full matrix by computing its effect on each
    // computational basis state directly.

    size_t n = num_qubits_;
    Matrix<Complex> full(dim_, dim_);

    // Determine the bit positions
    size_t bit1 = n - 1 - q1;  // Bit position of qubit q1
    size_t bit2 = n - 1 - q2;  // Bit position of qubit q2

    for (size_t col = 0; col < dim_; ++col) {
        // Extract the two-qubit sub-state from the column index
        size_t b1 = (col >> bit1) & 1;  // Value of qubit q1
        size_t b2 = (col >> bit2) & 1;  // Value of qubit q2
        size_t sub_col = (b1 << 1) | b2;  // Two-qubit index (0-3)

        for (size_t sub_row = 0; sub_row < 4; ++sub_row) {
            Complex val = gate(sub_row, sub_col);
            if (std::abs(val) < EPSILON) continue;

            // Map sub_row back to full state
            size_t new_b1 = (sub_row >> 1) & 1;
            size_t new_b2 = sub_row & 1;

            // Construct the row index: same as col but with q1,q2 replaced
            size_t row = col;
            // Clear bits at positions bit1 and bit2
            row &= ~(1ULL << bit1);
            row &= ~(1ULL << bit2);
            // Set new bits
            row |= (new_b1 << bit1);
            row |= (new_b2 << bit2);

            full(row, col) = full(row, col) + val;
        }
    }

    return full;
}

void Circuit::apply_to_state(const Matrix<Complex>& full_gate) {
    state_ = full_gate * state_;
}

void Circuit::check_qubit(size_t q, const char* gate_name) const {
    if (q >= num_qubits_)
        throw std::out_of_range(
            std::string(gate_name) + ": qubit " + std::to_string(q) +
            " out of range (circuit has " +
            std::to_string(num_qubits_) + " qubits)");
}

} // namespace phx
