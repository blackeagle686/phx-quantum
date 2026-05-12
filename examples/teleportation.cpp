// PHX-Quantum :: Quantum Teleportation Protocol
// Teleports the state of qubit 0 to qubit 2 using entanglement
#include "quantum/circuit.h"
#include "quantum/measurement.h"
#include "quantum/gates.h"
#include <iostream>

int main() {
    using namespace phx;

    std::cout << "=== Quantum Teleportation Protocol ===\n\n";
    std::cout << "Goal: Teleport qubit 0's state to qubit 2\n\n";

    // We'll teleport the state H|0> = |+> from q0 to q2
    // Step 1: Prepare the state to teleport on q0
    // Step 2: Create Bell pair between q1 and q2
    // Step 3: Bell measurement on q0,q1
    // Step 4: Apply corrections to q2

    Circuit c(3);

    // Prepare state to teleport: apply H to q0
    // q0 is now in |+> = (|0>+|1>)/sqrt(2)
    c.h(0);

    // Create Bell pair between q1 and q2
    c.h(1);
    c.cnot(1, 2);

    // Bell measurement: CNOT(q0,q1) then H(q0)
    c.cnot(0, 1);
    c.h(0);

    // At this point, measurement of q0,q1 would determine corrections.
    // In a real protocol, we'd measure and conditionally apply gates.
    // For simulation, we show the full state vector.

    c.execute();

    std::cout << "Circuit:\n";
    c.print_circuit(std::cout);
    std::cout << "\nFinal state vector:\n";
    c.print_state(std::cout);

    std::cout << "\nNote: In a full teleportation protocol, measuring q0 and q1\n";
    std::cout << "determines which correction gates (X, Z) to apply to q2.\n";
    std::cout << "The state of q0 (|+>) has been 'spread' across the system.\n\n";

    // Run shots to show the distribution
    std::cout << "Measurement distribution (10000 shots):\n";
    auto results = measurement::run_shots(c, 10000);
    measurement::print_histogram(results, 10000, std::cout);

    return 0;
}
