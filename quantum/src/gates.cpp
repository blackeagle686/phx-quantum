// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Gate Implementations
// ═══════════════════════════════════════════════════════════════════

#include "quantum/gates.h"
#include <cmath>

namespace phx::gates {

// ─── Single-Qubit Gates ──────────────────────────────────────────

Matrix<Complex> I() {
    return Matrix<Complex>(2, 2, {
        C_ONE,  C_ZERO,
        C_ZERO, C_ONE
    });
}

Matrix<Complex> X() {
    return Matrix<Complex>(2, 2, {
        C_ZERO, C_ONE,
        C_ONE,  C_ZERO
    });
}

Matrix<Complex> Y() {
    return Matrix<Complex>(2, 2, {
        C_ZERO, C_NEG_I,
        C_I,    C_ZERO
    });
}

Matrix<Complex> Z() {
    return Matrix<Complex>(2, 2, {
        C_ONE,  C_ZERO,
        C_ZERO, C_NEG
    });
}

Matrix<Complex> H() {
    Complex s = Complex{SQRT2_INV, 0.0};
    return Matrix<Complex>(2, 2, {
        s,  s,
        s, -s
    });
}

Matrix<Complex> S() {
    return Matrix<Complex>(2, 2, {
        C_ONE,  C_ZERO,
        C_ZERO, C_I
    });
}

Matrix<Complex> T() {
    Complex t_phase = std::exp(Complex{0.0, PI / 4.0});
    return Matrix<Complex>(2, 2, {
        C_ONE,  C_ZERO,
        C_ZERO, t_phase
    });
}

Matrix<Complex> Rx(double theta) {
    double c = std::cos(theta / 2.0);
    double s = std::sin(theta / 2.0);
    return Matrix<Complex>(2, 2, {
        Complex{c, 0},    Complex{0, -s},
        Complex{0, -s},   Complex{c, 0}
    });
}

Matrix<Complex> Ry(double theta) {
    double c = std::cos(theta / 2.0);
    double s = std::sin(theta / 2.0);
    return Matrix<Complex>(2, 2, {
        Complex{c, 0},   Complex{-s, 0},
        Complex{s, 0},   Complex{c, 0}
    });
}

Matrix<Complex> Rz(double theta) {
    Complex neg = std::exp(Complex{0.0, -theta / 2.0});
    Complex pos = std::exp(Complex{0.0,  theta / 2.0});
    return Matrix<Complex>(2, 2, {
        neg,    C_ZERO,
        C_ZERO, pos
    });
}

// ─── Two-Qubit Gates ─────────────────────────────────────────────

Matrix<Complex> CNOT() {
    return Matrix<Complex>(4, 4, {
        C_ONE,  C_ZERO, C_ZERO, C_ZERO,
        C_ZERO, C_ONE,  C_ZERO, C_ZERO,
        C_ZERO, C_ZERO, C_ZERO, C_ONE,
        C_ZERO, C_ZERO, C_ONE,  C_ZERO
    });
}

Matrix<Complex> SWAP() {
    return Matrix<Complex>(4, 4, {
        C_ONE,  C_ZERO, C_ZERO, C_ZERO,
        C_ZERO, C_ZERO, C_ONE,  C_ZERO,
        C_ZERO, C_ONE,  C_ZERO, C_ZERO,
        C_ZERO, C_ZERO, C_ZERO, C_ONE
    });
}

Matrix<Complex> CZ() {
    return Matrix<Complex>(4, 4, {
        C_ONE,  C_ZERO, C_ZERO, C_ZERO,
        C_ZERO, C_ONE,  C_ZERO, C_ZERO,
        C_ZERO, C_ZERO, C_ONE,  C_ZERO,
        C_ZERO, C_ZERO, C_ZERO, C_NEG
    });
}

// ─── Gate Application ────────────────────────────────────────────

Matrix<Complex> apply(const Matrix<Complex>& gate,
                      const Matrix<Complex>& state) {
    return gate * state;
}

} // namespace phx::gates
