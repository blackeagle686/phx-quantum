// PHX-Quantum :: Bell State Example
// Creates the maximally entangled Bell state: (|00> + |11>) / sqrt(2)
#include "quantum/circuit.h"
#include "quantum/measurement.h"
#include <iostream>

int main() {
    using namespace phx;

    std::cout << "=== Bell State (Entanglement) ===\n\n";

    Circuit c(2);
    c.h(0);         // Put qubit 0 in superposition
    c.cnot(0, 1);   // Entangle qubit 0 and 1
    c.execute();

    std::cout << "Circuit:\n";
    c.print_circuit(std::cout);
    std::cout << "\nFinal state:\n";
    c.print_state(std::cout);

    std::cout << "\nRunning 10000 measurements:\n";
    auto results = measurement::run_shots(c, 10000);
    measurement::print_histogram(results, 10000, std::cout);

    return 0;
}
