#include "shopl/core/GravityIntegrator.h"

#include <cassert>
#include <cmath>
#include <limits>

namespace {

bool near(const double actual, const double expected) {
    return std::abs(actual - expected) < 1e-12;
}

} // namespace

int main() {
    using shopl::core::GravityIntegrator;
    using shopl::core::ParticleState;
    using shopl::core::Vec3;

    ParticleState particle{{0.0, 10.0, 0.0}, {1.0, 0.0, 0.0}};
    assert(GravityIntegrator::step(particle, {0.0, -10.0, 0.0}, 0.1));
    assert(near(particle.velocity.x, 1.0));
    assert(near(particle.velocity.y, -1.0));
    assert(near(particle.position.x, 0.1));
    assert(near(particle.position.y, 9.9));

    const ParticleState before = particle;
    assert(!GravityIntegrator::step(particle, {0.0, -9.8, 0.0}, 0.0));
    assert(!GravityIntegrator::step(particle, {0.0, -9.8, 0.0}, -0.1));
    assert(!GravityIntegrator::step(
        particle, {0.0, -9.8, 0.0},
        std::numeric_limits<double>::infinity()));
    assert(particle.position.x == before.position.x);
    assert(particle.position.y == before.position.y);
    assert(particle.velocity.x == before.velocity.x);
    assert(particle.velocity.y == before.velocity.y);

    ParticleState invalid{{std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0}, {}};
    assert(!GravityIntegrator::step(invalid, {0.0, -9.8, 0.0}, 0.016));

    ParticleState overflow{{0.0, 0.0, 0.0},
                           {std::numeric_limits<double>::max(), 0.0, 0.0}};
    assert(!GravityIntegrator::step(overflow, {0.0, 0.0, 0.0}, 2.0));

    const Vec3 sum = Vec3{1.0, 2.0, 3.0} + Vec3{2.0, 3.0, 4.0};
    assert(sum.x == 3.0 && sum.y == 5.0 && sum.z == 7.0);
    return 0;
}
