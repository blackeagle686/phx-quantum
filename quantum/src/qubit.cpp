// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Qubit Implementation
// ═══════════════════════════════════════════════════════════════════

#include "quantum/qubit.h"
#include <iomanip>

namespace phx {

Qubit::Qubit()
    : state_(Matrix<Complex>::column({C_ONE, C_ZERO}))  // |0⟩
{}

Qubit::Qubit(Complex alpha, Complex beta)
    : state_(Matrix<Complex>::column({alpha, beta}))
{
    normalize();
}

Qubit Qubit::zero()  { return Qubit(); }
Qubit Qubit::one()   { return Qubit(C_ZERO, C_ONE); }
Qubit Qubit::plus()  { return Qubit(Complex{SQRT2_INV}, Complex{SQRT2_INV}); }
Qubit Qubit::minus() { return Qubit(Complex{SQRT2_INV}, Complex{-SQRT2_INV}); }

const Matrix<Complex>& Qubit::state() const { return state_; }

void Qubit::set_state(const Matrix<Complex>& s) {
    if (s.rows() != 2 || s.cols() != 1)
        throw std::invalid_argument("Qubit state must be a 2x1 vector");
    state_ = s;
}

Complex Qubit::alpha() const { return state_(0, 0); }
Complex Qubit::beta()  const { return state_(1, 0); }

double Qubit::prob_zero() const { return std::norm(alpha()); }
double Qubit::prob_one()  const { return std::norm(beta()); }

void Qubit::normalize() {
    linalg::normalize(state_);
}

void Qubit::print(std::ostream& os) const {
    Complex a = alpha(), b = beta();
    os << "|ψ⟩ = ";

    // Pretty-print amplitude for |0⟩
    if (std::abs(a) > EPSILON) {
        os << "(" << std::setprecision(4) << a.real();
        if (std::abs(a.imag()) > EPSILON)
            os << (a.imag() >= 0 ? "+" : "") << a.imag() << "i";
        os << ")|0>";
    }

    // Pretty-print amplitude for |1⟩
    if (std::abs(b) > EPSILON) {
        if (std::abs(a) > EPSILON)
            os << " + ";
        os << "(" << std::setprecision(4) << b.real();
        if (std::abs(b.imag()) > EPSILON)
            os << (b.imag() >= 0 ? "+" : "") << b.imag() << "i";
        os << ")|1>";
    }

    os << "   [P(0)=" << std::setprecision(4) << prob_zero()
       << ", P(1)=" << prob_one() << "]";
}

} // namespace phx
