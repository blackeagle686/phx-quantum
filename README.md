# PHX-Quantum ⚛️


**Pure CPU Quantum State Simulator** — A from-scratch quantum computing simulator built in C++17 with no external dependencies.

Simulates 1–16 qubit quantum systems with full gate support, circuit execution, and measurement simulation using Born's rule.

## Features

- **Matrix Engine** — Dense `Matrix<T>` with complex arithmetic, Kronecker product, adjoint, norm
- **Quantum Gates** — I, X, Y, Z, H, S, T, Rx, Ry, Rz, CNOT, SWAP, CZ
- **Circuit System** — Builder pattern, step-by-step execution, arbitrary qubit targeting
- **Measurement** — Probabilistic collapse, shot-based simulation, histogram output
- **Tensor Core** — N-dimensional tensor with reshape, element-wise ops
- **CLI** — Interactive circuit builder + pre-built demos (Bell, GHZ, Superposition)

## Quick Start

### Build
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

### Run Tests
```bash
cd build && ctest --output-on-failure -C Release
```

### Run Demos
```bash
# Bell State (entanglement)
./build/runtime/Release/phx-quantum.exe bell --shots 1000

# GHZ State (3-qubit entanglement)
./build/runtime/Release/phx-quantum.exe ghz --shots 1000

# Superposition Demo
./build/runtime/Release/phx-quantum.exe superposition

# Interactive Circuit Builder
./build/runtime/Release/phx-quantum.exe custom
```

## Architecture

```
phx-quantum/
├── core/       # Type aliases (Complex), RNG utilities
├── math/       # Matrix<T> template, Kronecker, norm, inner product
├── quantum/    # Qubit, Gates, Circuit, Measurement
├── tensor/     # N-dimensional Tensor<T>
├── runtime/    # CLI entry point
├── examples/   # Bell state, Teleportation demos
└── tests/      # Unit tests for every module
```

## Example: Bell State in C++

```cpp
#include "quantum/circuit.h"
#include "quantum/measurement.h"

int main() {
    phx::Circuit c(2);
    c.h(0);           // Superposition on qubit 0
    c.cnot(0, 1);     // Entangle qubits 0 and 1
    c.execute();

    // Run 10000 measurements
    auto results = phx::measurement::run_shots(c, 10000);
    phx::measurement::print_histogram(results, 10000);
    // Output: ~50% |00⟩, ~50% |11⟩
}
```

## License

MIT License — Copyright (c) 2026 Mohammed Alaa
