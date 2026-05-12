// ═══════════════════════════════════════════════════════════════════
//  PHX-Quantum :: RNG Implementation
// ═══════════════════════════════════════════════════════════════════

#include "core/random.h"

namespace phx {

RNG& RNG::instance() {
    static RNG rng;
    return rng;
}

RNG::RNG()
    : engine_(std::random_device{}())
    , dist_(0.0, 1.0)
{}

double RNG::uniform() {
    return dist_(engine_);
}

int RNG::uniform_int(int max) {
    std::uniform_int_distribution<int> idist(0, max - 1);
    return idist(engine_);
}

void RNG::seed(unsigned int s) {
    engine_.seed(s);
}

} // namespace phx
