#include "shopl/core/GravityIntegrator.h"

#include <cmath>

namespace shopl::core {

bool GravityIntegrator::step(ParticleState& particle,
                             const Vec3& acceleration,
                             const double delta_seconds) noexcept {
    if (!std::isfinite(delta_seconds) || delta_seconds <= 0.0 ||
        !particle.position.is_finite() ||
        !particle.velocity.is_finite() ||
        !acceleration.is_finite()) {
        return false;
    }

    const Vec3 next_velocity = particle.velocity + acceleration * delta_seconds;
    const Vec3 next_position = particle.position + next_velocity * delta_seconds;

    if (!next_velocity.is_finite() || !next_position.is_finite()) {
        return false;
    }

    particle.velocity = next_velocity;
    particle.position = next_position;
    return true;
}

} // namespace shopl::core
