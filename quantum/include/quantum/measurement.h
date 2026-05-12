#pragma once
// PHX-Quantum :: Measurement Simulation
#include "math/matrix.h"
#include "quantum/circuit.h"
#include "core/types.h"
#include <map>
#include <string>
#include <vector>

namespace phx::measurement {

struct QubitResult {
    int outcome;
    double probability;
    Matrix<Complex> post_state;
};

struct FullResult {
    int outcome;
    std::string bits;
    double probability;
};

QubitResult measure_qubit(Matrix<Complex>& state, size_t qubit_index, size_t num_qubits);
FullResult measure_all(Matrix<Complex>& state, size_t num_qubits);
FullResult sample(const Matrix<Complex>& state, size_t num_qubits);
std::map<std::string, int> run_shots(Circuit& circuit, size_t shots);
void print_histogram(const std::map<std::string, int>& results, size_t total_shots, std::ostream& os = std::cout);
std::vector<double> probabilities(const Matrix<Complex>& state);
std::string to_bitstring(size_t index, size_t num_qubits);

} // namespace phx::measurement
