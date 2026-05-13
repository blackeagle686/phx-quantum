# Measurement & Wavefunction Collapse 📉

Measurement is the bridge between the quantum world (amplitudes) and the classical world (probabilities).

## 1. Born's Rule
The probability $P(i)$ of measuring a system in state $|i\rangle$ is the square of the absolute value of its amplitude:

$$P(i) = |\langle i | \psi \rangle|^2$$

Example: For $|\psi\rangle = \frac{1}{\sqrt{2}}|0\rangle + \frac{i}{\sqrt{2}}|1\rangle$
- $P(0) = |\frac{1}{\sqrt{2}}|^2 = 0.5$
- $P(1) = |\frac{i}{\sqrt{2}}|^2 = 0.5$

## 2. Wavefunction Collapse
Once a measurement is performed, the quantum state "collapses" into the observed basis state. All other possibilities vanish.
- If we measure `0`, the new state becomes $|\psi_{new}\rangle = |0\rangle$.
- Sequential measurements of the same qubit will always yield the same result.

## 3. Shot-based Simulation
In PHX-Quantum, we simulate measurement in two ways:
1. **Perfect Probability**: Calculating the exact $|\alpha|^2$ for each state.
2. **Monte Carlo Sampling (Shots)**: Using a random number generator to pick an outcome based on the probability distribution $N$ times. This mimics a real quantum computer.

## 4. Partial Measurement
The simulator supports measuring a subset of qubits. When qubit $k$ is measured:
1. Calculate probability of $0$ vs $1$ for qubit $k$ by summing squares of amplitudes of all states where qubit $k$ is $0/1$.
2. Randomly select the outcome.
3. **Renormalize** the remaining amplitudes so the total probability is still 1.

---
*Next: [Math Engine](math_engine.md)*
