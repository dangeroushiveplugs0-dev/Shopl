#pragma once

#include "shopl/core/ParticleState.h"

namespace shopl::core {

// A small solver-independent baseline integrator.
// Uses semi-implicit Euler: velocity is updated before position.
class GravityIntegrator {
public:
    // Returns false and leaves the particle unchanged for invalid input or
    // if the proposed result would contain non-finite coordinates.
    static bool step(ParticleState& particle,
                     const Vec3& acceleration,
                     double delta_seconds) noexcept;
};

} // namespace shopl::core
