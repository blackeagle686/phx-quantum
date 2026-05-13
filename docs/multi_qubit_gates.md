# Multi-Qubit Gates & Entanglement 🔗

Multi-qubit gates allow qubits to interact, creating **Entanglement**, where the state of one qubit cannot be described independently of the others.

## 1. Controlled-NOT (CNOT)
The CNOT gate flips the target qubit if and only if the control qubit is $|1\rangle$.

$$CNOT = \begin{bmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 0 & 1 \\ 0 & 0 & 1 & 0 \end{bmatrix}$$

### Physics of Entanglement
Applying $H$ on qubit 0 followed by $CNOT(0, 1)$ creates the **Bell State**:
$$|\Phi^+\rangle = \frac{|00\rangle + |11\rangle}{\sqrt{2}}$$
In this state, measuring qubit 0 immediately tells you the state of qubit 1, regardless of the distance between them.

## 2. SWAP Gate
Exchanges the states of two qubits.
$$SWAP = \begin{bmatrix} 1 & 0 & 0 & 0 \\ 0 & 0 & 1 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 0 & 1 \end{bmatrix}$$

## 3. Controlled-Z (CZ)
Applies a $Z$ gate to the target if the control is $|1\rangle$. It is symmetric: $CZ(0,1) = CZ(1,0)$.
$$CZ = \begin{bmatrix} 1 & 0 & 0 & 0 \\ 0 & 1 & 0 & 0 \\ 0 & 0 & 1 & 0 \\ 0 & 0 & 0 & -1 \end{bmatrix}$$

## 4. Implementation in PHX-Quantum
To apply a gate to specific qubits in a large system, we use **Kronecker Decomposition**. 
If we have 3 qubits and apply $X$ to qubit 1:
$$TotalOp = I \otimes X \otimes I$$
This results in an $8 \times 8$ matrix.

---
*Next: [Measurement](measurement.md)*
