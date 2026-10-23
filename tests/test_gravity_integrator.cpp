#include "shopl/core/GravityIntegrator.h"

#include <cmath>
#include <iostream>
#include <limits>

#define CHECK(condition) do { \
    if (!(condition)) { \
        std::cerr << "Check failed at line " << __LINE__ << ": " #condition "\n"; \
        return 1; \
    } \
} while (false)

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
    CHECK(GravityIntegrator::step(particle, {0.0, -10.0, 0.0}, 0.1));
    CHECK(near(particle.velocity.x, 1.0));
    CHECK(near(particle.velocity.y, -1.0));
    CHECK(near(particle.position.x, 0.1));
    CHECK(near(particle.position.y, 9.9));

    const ParticleState before = particle;
    CHECK(!GravityIntegrator::step(particle, {0.0, -9.8, 0.0}, 0.0));
    CHECK(!GravityIntegrator::step(particle, {0.0, -9.8, 0.0}, -0.1));
    CHECK(!GravityIntegrator::step(
        particle, {0.0, -9.8, 0.0},
        std::numeric_limits<double>::infinity()));
    CHECK(particle.position.x == before.position.x);
    CHECK(particle.position.y == before.position.y);
    CHECK(particle.velocity.x == before.velocity.x);
    CHECK(particle.velocity.y == before.velocity.y);

    ParticleState invalid{{std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0}, {}};
    CHECK(!GravityIntegrator::step(invalid, {0.0, -9.8, 0.0}, 0.016));

    ParticleState overflow{{0.0, 0.0, 0.0},
                           {std::numeric_limits<double>::max(), 0.0, 0.0}};
    CHECK(!GravityIntegrator::step(overflow, {0.0, 0.0, 0.0}, 2.0));

    const Vec3 sum = Vec3{1.0, 2.0, 3.0} + Vec3{2.0, 3.0, 4.0};
    CHECK(sum.x == 3.0 && sum.y == 5.0 && sum.z == 7.0);
    return 0;
}
