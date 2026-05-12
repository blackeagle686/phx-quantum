#pragma once
// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Core Type Definitions
// ═══════════════════════════════════════════════════════════════════

#include <complex>
#include <cmath>

namespace phx {

/// Primary complex number type used throughout the simulator
using Complex = std::complex<double>;

/// Mathematical constants
constexpr double PI         = 3.14159265358979323846;
constexpr double SQRT2      = 1.4142135623730951;
constexpr double SQRT2_INV  = 0.7071067811865475;  // 1/√2
constexpr double EPSILON    = 1e-10;                // Floating-point comparison tolerance

/// Convenience literals for common complex values
inline constexpr Complex C_ZERO  {0.0, 0.0};
inline constexpr Complex C_ONE   {1.0, 0.0};
inline constexpr Complex C_NEG   {-1.0, 0.0};
inline constexpr Complex C_I     {0.0, 1.0};       // Imaginary unit
inline constexpr Complex C_NEG_I {0.0, -1.0};

/// Compare two doubles with tolerance
inline bool approx_equal(double a, double b, double eps = EPSILON) {
    return std::abs(a - b) < eps;
}

/// Compare two complex numbers with tolerance
inline bool approx_equal(Complex a, Complex b, double eps = EPSILON) {
    return std::abs(a - b) < eps;
}

} // namespace phx
