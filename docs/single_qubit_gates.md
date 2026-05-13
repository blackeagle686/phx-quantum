# Single-Qubit Gates 🛠️

Quantum gates are **Unitary Operators**. A matrix $U$ is unitary if $U^\dagger U = I$. Physically, this ensures that the total probability remains 1 after the transformation (information is preserved).

## 1. Pauli Gates (Rotations around Axes)

### Pauli-X (NOT Gate)
Flips $|0\rangle \leftrightarrow |1\rangle$. It is a $\pi$ rotation around the X-axis of the Bloch sphere.
$$X = \begin{bmatrix} 0 & 1 \\ 1 & 0 \end{bmatrix}$$

### Pauli-Y
A $\pi$ rotation around the Y-axis. Introduces a complex phase.
$$Y = \begin{bmatrix} 0 & -i \\ i & 0 \end{bmatrix}$$

### Pauli-Z (Phase Flip)
Leaves $|0\rangle$ unchanged but flips the sign of $|1\rangle$.
$$Z = \begin{bmatrix} 1 & 0 \\ 0 & -1 \end{bmatrix}$$

## 2. The Hadamard Gate (H)
The most important gate for creating **superposition**. It rotates the state around the $X+Z$ axis, mapping $|0\rangle \to |+\rangle$ and $|1\rangle \to |-\rangle$.

$$H = \frac{1}{\sqrt{2}}\begin{bmatrix} 1 & 1 \\ 1 & -1 \end{bmatrix}$$

$$H|0\rangle = \frac{|0\rangle + |1\rangle}{\sqrt{2}}$$

## 3. Phase Gates (S and T)
These gates rotate the state around the Z-axis by specific angles.

- **S Gate** (Phase gate): $\pi/2$ rotation ($i$).
  $$S = \begin{bmatrix} 1 & 0 \\ 0 & i \end{bmatrix}$$
- **T Gate**: $\pi/4$ rotation ($e^{i\pi/4}$).
  $$T = \begin{bmatrix} 1 & 0 \\ 0 & e^{i\pi/4} \end{bmatrix}$$

## 4. General Rotations ($R_x, R_y, R_z$)
The simulator supports arbitrary rotations by an angle $\theta$:
$$R_z(\theta) = \begin{bmatrix} e^{-i\theta/2} & 0 \\ 0 & e^{i\theta/2} \end{bmatrix}$$

---
*Next: [Multi-Qubit Gates](multi_qubit_gates.md)*
