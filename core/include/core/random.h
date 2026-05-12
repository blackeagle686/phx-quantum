#pragma once
// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: Random Number Generator Utilities
// ═══════════════════════════════════════════════════════════════════

#include <random>

namespace phx {

/// Thread-local Mersenne Twister RNG for measurement simulation.
/// Returns a uniform double in [0, 1).
class RNG {
public:
    /// Get the singleton instance
    static RNG& instance();

    /// Generate uniform random double in [0, 1)
    double uniform();

    /// Generate uniform random integer in [0, max)
    int uniform_int(int max);

    /// Seed the generator (for reproducible tests)
    void seed(unsigned int s);

private:
    RNG();
    std::mt19937 engine_;
    std::uniform_real_distribution<double> dist_;
};

} // namespace phx
