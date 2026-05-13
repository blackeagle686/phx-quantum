# PHX-Quantum Documentation ⚛️

Welcome to the technical documentation for **PHX-Quantum**, a pure CPU quantum state simulator. This guide covers the mathematical and physical foundations of quantum computing as implemented in this engine.

## 📚 Table of Contents

1. [Qubits & Quantum States](qubits_and_states.md)
   - The Bloch Sphere, Superposition, and Bra-Ket Notation.
2. [Single-Qubit Gates](single_qubit_gates.md)
   - Pauli Matrices (X, Y, Z), Hadamard (H), and Phase Gates (S, T).
3. [Multi-Qubit Gates & Entanglement](multi_qubit_gates.md)
   - CNOT, SWAP, and the physics of non-separability.
4. [Measurement & Probability](measurement.md)
   - Born's Rule and Wave-function Collapse.
5. [Math Engine: Kronecker Products](math_engine.md)
   - How the simulator scales state vectors and operators.

## The Core Concept

PHX-Quantum simulates quantum mechanics using **State Vector Simulation**. 
- A system of $n$ qubits is represented by a complex vector of size $2^n$.
- Quantum gates are represented by $2^n \times 2^n$ Unitary Matrices.
- Circuit execution is the process of sequential matrix-vector multiplication.

---
*Next: [Learn about Qubits and States](qubits_and_states.md)*
