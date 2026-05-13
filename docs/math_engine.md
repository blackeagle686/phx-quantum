# Math Engine: The Kronecker Product 🧮

The core of PHX-Quantum is a high-performance matrix engine designed for **Kronecker Products** (Tensor Products).

## 1. Definition
The Kronecker product of an $m \times n$ matrix $A$ and a $p \times q$ matrix $B$ is an $mp \times nq$ matrix $C$:

$$A \otimes B = \begin{bmatrix} a_{11}B & \cdots & a_{1n}B \\ \vdots & \ddots & \vdots \\ a_{m1}B & \cdots & a_{mn}B \end{bmatrix}$$

## 2. Why it matters
In quantum mechanics, if we have a gate $G$ acting on one qubit in an $N$-qubit system, the operator for the entire system is:
$$I \otimes I \otimes \dots \otimes G \otimes \dots \otimes I$$

## 3. Computational Complexity
- **Storage**: A system of $n$ qubits requires $2^n$ complex numbers. 
  - 10 qubits = 1,024 complex numbers.
  - 20 qubits = 1,048,576 complex numbers.
  - 30 qubits = 1,073,741,824 complex numbers (~16 GB RAM).
- **Runtime**: Each gate application is an $O(2^{2n})$ operation if using full matrices, but PHX-Quantum optimizes this to $O(2^n)$ by exploiting the sparsity of quantum operators.

## 4. Complex Arithmetic
The engine uses a custom `Complex` type with:
- Addition/Subtraction
- Multiplication (Real and Complex)
- Adjoint (Conjugate Transpose)
- Norm (Magnitude)

This ensures zero-dependency and maximum control over precision.
