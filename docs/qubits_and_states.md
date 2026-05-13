# Qubits and Quantum States 🌌

In classical computing, a bit is either `0` or `1`. In quantum computing, the fundamental unit is the **Qubit**.

## 1. Mathematical Representation

A qubit state $|\psi\rangle$ is represented as a unit vector in a 2D complex Hilbert space:

$$|\psi\rangle = \alpha|0\rangle + \beta|1\rangle$$

Where:
- $|0\rangle = \begin{bmatrix} 1 \\ 0 \end{bmatrix}$ (The ground state)
- $|1\rangle = \begin{bmatrix} 0 \\ 1 \end{bmatrix}$ (The excited state)
- $\alpha, \beta \in \mathbb{C}$ are **probability amplitudes**.

### Normalization Constraint
The total probability of finding the qubit in either state must be 1:
$$|\alpha|^2 + |\beta|^2 = 1$$

## 2. Superposition
Superposition is the ability of a quantum system to be in multiple states simultaneously. Unlike a classical bit, a qubit doesn't have a definite value of 0 or 1 until it is measured. It exists as a linear combination of basis states.

## 3. The Bloch Sphere
The state of a single qubit can be visualized as a point on a unit sphere (the Bloch Sphere). Any pure state can be written as:

$$|\psi\rangle = \cos(\frac{\theta}{2})|0\rangle + e^{i\phi}\sin(\frac{\theta}{2})|1\rangle$$

- $\theta \in [0, \pi]$ (Latitude)
- $\phi \in [0, 2\pi]$ (Longitude/Phase)

### Physical Significance
- The North Pole is $|0\rangle$.
- The South Pole is $|1\rangle$.
- The Equator represents equal superpositions (like the $|+\rangle$ state).

## 4. Multi-Qubit States (Tensor Products)
When we have two qubits, the system state is the **Kronecker product** of individual states:

$$|q_0\rangle \otimes |q_1\rangle$$

For 2 qubits, the state vector has $2^2 = 4$ dimensions:
- $|00\rangle, |01\rangle, |10\rangle, |11\rangle$

---
*Next: [Single-Qubit Gates](single_qubit_gates.md)*
