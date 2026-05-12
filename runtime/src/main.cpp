// =====================================================================
//  PHX-Quantum :: CLI Entry Point
// =====================================================================
#include "quantum/qubit.h"
#include "quantum/gates.h"
#include "quantum/circuit.h"
#include "quantum/measurement.h"
#include "tensor/tensor.h"
#include "core/types.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace phx;

static void print_banner() {
    std::cout << R"(
  ╔═══════════════════════════════════════════════════════════╗
  ║                                                           ║
  ║   ██████╗ ██╗  ██╗██╗  ██╗       ██████╗ ██╗   ██╗       ║
  ║   ██╔══██╗██║  ██║╚██╗██╔╝      ██╔═══██╗██║   ██║       ║
  ║   ██████╔╝███████║ ╚███╔╝ █████╗██║   ██║██║   ██║       ║
  ║   ██╔═══╝ ██╔══██║ ██╔██╗ ╚════╝██║▄▄ ██║██║   ██║       ║
  ║   ██║     ██║  ██║██╔╝ ██╗      ╚██████╔╝╚██████╔╝       ║
  ║   ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝       ╚══▀▀═╝  ╚═════╝       ║
  ║                                                           ║
  ║          Pure CPU Quantum State Simulator v0.1            ║
  ║                                                           ║
  ╚═══════════════════════════════════════════════════════════╝
)" << std::endl;
}

static void run_bell_state(size_t shots) {
    std::cout << "  === Bell State Circuit ===\n\n";
    std::cout << "  Circuit: H(q0) -> CNOT(q0, q1) -> Measure\n";
    std::cout << "  Expected: ~50% |00>, ~50% |11> (entanglement)\n\n";

    Circuit c(2);
    c.h(0);
    c.cnot(0, 1);

    // Show the state before measurement
    c.execute();
    c.print_state(std::cout);
    std::cout << "\n";

    // Run shots
    auto results = measurement::run_shots(c, shots);
    measurement::print_histogram(results, shots, std::cout);
}

static void run_superposition_demo() {
    std::cout << "  === Superposition Demo ===\n\n";

    std::cout << "  Step 1: Create qubit |0>\n";
    Qubit q = Qubit::zero();
    std::cout << "  " << q << "\n\n";

    std::cout << "  Step 2: Apply Hadamard gate\n";
    auto new_state = gates::apply(gates::H(), q.state());
    q.set_state(new_state);
    std::cout << "  " << q << "\n\n";

    std::cout << "  Step 3: Apply Hadamard again (should return to |0>)\n";
    new_state = gates::apply(gates::H(), q.state());
    q.set_state(new_state);
    std::cout << "  " << q << "\n\n";
}

static void run_ghz_demo(size_t shots) {
    std::cout << "  === GHZ State (3-qubit entanglement) ===\n\n";
    std::cout << "  Circuit: H(q0) -> CNOT(q0,q1) -> CNOT(q1,q2)\n";
    std::cout << "  Expected: ~50% |000>, ~50% |111>\n\n";

    Circuit c(3);
    c.h(0);
    c.cnot(0, 1);
    c.cnot(1, 2);
    c.execute();
    c.print_state(std::cout);
    std::cout << "\n";

    auto results = measurement::run_shots(c, shots);
    measurement::print_histogram(results, shots, std::cout);
}

static void run_custom_interactive() {
    std::cout << "  === Interactive Circuit Builder ===\n\n";
    std::cout << "  Enter number of qubits (1-4): ";
    size_t nq;
    std::cin >> nq;
    if (nq < 1 || nq > 4) { std::cout << "  Invalid. Using 2.\n"; nq = 2; }

    Circuit c(nq);
    std::string cmd;

    std::cout << "\n  Commands: h/x/y/z/s/t <qubit>, cnot <ctrl> <tgt>,\n";
    std::cout << "            swap <q1> <q2>, run, measure <shots>, quit\n\n";

    while (true) {
        std::cout << "  phx> ";
        std::cin >> cmd;
        if (cmd == "quit" || cmd == "q") break;

        if (cmd == "h" || cmd == "x" || cmd == "y" || cmd == "z" ||
            cmd == "s" || cmd == "t") {
            size_t q; std::cin >> q;
            if (cmd == "h") c.h(q);
            else if (cmd == "x") c.x(q);
            else if (cmd == "y") c.y(q);
            else if (cmd == "z") c.z(q);
            else if (cmd == "s") c.s(q);
            else if (cmd == "t") c.t(q);
            std::cout << "  Added " << cmd << "(q" << q << ")\n";
        }
        else if (cmd == "cnot") {
            size_t ctrl, tgt; std::cin >> ctrl >> tgt;
            c.cnot(ctrl, tgt);
            std::cout << "  Added CNOT(q" << ctrl << ", q" << tgt << ")\n";
        }
        else if (cmd == "swap") {
            size_t q1, q2; std::cin >> q1 >> q2;
            c.swap(q1, q2);
            std::cout << "  Added SWAP(q" << q1 << ", q" << q2 << ")\n";
        }
        else if (cmd == "run") {
            c.execute();
            std::cout << "\n";
            c.print_circuit(std::cout);
            std::cout << "\n";
            c.print_state(std::cout);
            std::cout << "\n";
        }
        else if (cmd == "measure") {
            size_t shots; std::cin >> shots;
            auto results = measurement::run_shots(c, shots);
            measurement::print_histogram(results, shots, std::cout);
        }
        else {
            std::cout << "  Unknown command: " << cmd << "\n";
        }
    }
}

static void print_usage() {
    std::cout << "  Usage: phx-quantum <command> [--shots N]\n\n";
    std::cout << "  Commands:\n";
    std::cout << "    bell          Bell state (H -> CNOT -> Measure)\n";
    std::cout << "    superposition Superposition demo (H gate)\n";
    std::cout << "    ghz           GHZ state (3-qubit entanglement)\n";
    std::cout << "    custom        Interactive circuit builder\n";
    std::cout << "    help          Show this help\n\n";
    std::cout << "  Options:\n";
    std::cout << "    --shots N     Number of measurement shots (default: 1000)\n\n";
}

int main(int argc, char* argv[]) {
    print_banner();

    std::string command = "help";
    size_t shots = 1000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--shots" && i + 1 < argc) {
            shots = std::stoul(argv[++i]);
        } else if (arg[0] != '-') {
            command = arg;
        }
    }

    if (command == "bell") run_bell_state(shots);
    else if (command == "superposition") run_superposition_demo();
    else if (command == "ghz") run_ghz_demo(shots);
    else if (command == "custom") run_custom_interactive();
    else print_usage();

    return 0;
}
