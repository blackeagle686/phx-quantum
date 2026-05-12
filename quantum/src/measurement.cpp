// PHX-Quantum :: Measurement Implementation
#include "quantum/measurement.h"
#include "core/random.h"
#include <cmath>
#include <iomanip>
#include <algorithm>

namespace phx::measurement {

std::string to_bitstring(size_t index, size_t num_qubits) {
    std::string s(num_qubits, '0');
    for (int b = static_cast<int>(num_qubits) - 1; b >= 0; --b) {
        s[num_qubits - 1 - b] = ((index >> b) & 1) ? '1' : '0';
    }
    return s;
}

std::vector<double> probabilities(const Matrix<Complex>& state) {
    std::vector<double> probs(state.rows());
    for (size_t i = 0; i < state.rows(); ++i)
        probs[i] = std::norm(state(i, 0));
    return probs;
}

QubitResult measure_qubit(Matrix<Complex>& state, size_t qubit_index, size_t num_qubits) {
    size_t dim = state.rows();
    size_t bit_pos = num_qubits - 1 - qubit_index;

    // Calculate probability of measuring |0> for this qubit
    double p0 = 0.0;
    for (size_t i = 0; i < dim; ++i) {
        if (((i >> bit_pos) & 1) == 0)
            p0 += std::norm(state(i, 0));
    }

    // Random collapse
    double r = RNG::instance().uniform();
    int outcome = (r < p0) ? 0 : 1;
    double p_outcome = (outcome == 0) ? p0 : (1.0 - p0);

    // Collapse: zero out inconsistent amplitudes, renormalize
    Matrix<Complex> post(dim, 1);
    for (size_t i = 0; i < dim; ++i) {
        size_t bit_val = (i >> bit_pos) & 1;
        if (static_cast<int>(bit_val) == outcome)
            post(i, 0) = state(i, 0);
    }
    linalg::normalize(post);

    state = post;
    return {outcome, p_outcome, post};
}

FullResult measure_all(Matrix<Complex>& state, size_t num_qubits) {
    auto probs = probabilities(state);
    double r = RNG::instance().uniform();
    double cumulative = 0.0;
    size_t outcome = 0;

    for (size_t i = 0; i < probs.size(); ++i) {
        cumulative += probs[i];
        if (r < cumulative) { outcome = i; break; }
    }
    // Handle floating point edge case
    if (r >= cumulative) outcome = probs.size() - 1;

    double p = probs[outcome];
    std::string bits = to_bitstring(outcome, num_qubits);

    // Collapse to the measured state
    state = Matrix<Complex>(state.rows(), 1);
    state(outcome, 0) = C_ONE;

    return {static_cast<int>(outcome), bits, p};
}

FullResult sample(const Matrix<Complex>& state, size_t num_qubits) {
    auto probs = probabilities(state);
    double r = RNG::instance().uniform();
    double cumulative = 0.0;
    size_t outcome = 0;

    for (size_t i = 0; i < probs.size(); ++i) {
        cumulative += probs[i];
        if (r < cumulative) { outcome = i; break; }
    }
    if (r >= cumulative) outcome = probs.size() - 1;

    return {static_cast<int>(outcome),
            to_bitstring(outcome, num_qubits),
            probs[outcome]};
}

std::map<std::string, int> run_shots(Circuit& circuit, size_t shots) {
    std::map<std::string, int> results;
    for (size_t s = 0; s < shots; ++s) {
        circuit.execute();
        auto result = sample(circuit.state(), circuit.num_qubits());
        results[result.bits]++;
    }
    return results;
}

void print_histogram(const std::map<std::string, int>& results,
                     size_t total_shots, std::ostream& os) {
    // Find max count for bar scaling
    int max_count = 0;
    for (const auto& [bits, count] : results)
        max_count = std::max(max_count, count);

    const int bar_width = 40;

    os << "\n  Measurement Results (" << total_shots << " shots):\n";
    os << "  " << std::string(60, '-') << "\n";

    for (const auto& [bits, count] : results) {
        double pct = 100.0 * count / static_cast<double>(total_shots);
        int bar_len = static_cast<int>(
            bar_width * count / static_cast<double>(max_count));

        os << "  |" << bits << ">  ";
        os << std::string(bar_len, '#');
        os << std::string(bar_width - bar_len, ' ');
        os << "  " << std::setw(5) << count
           << " (" << std::fixed << std::setprecision(1)
           << pct << "%)\n";
    }
    os << "  " << std::string(60, '-') << "\n";
}

} // namespace phx::measurement
