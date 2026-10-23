#pragma once
#include <cstddef>
#include <vector>
#include "shopl/core/ParticleState.h"
namespace shopl::constraints {
struct DistanceConstraint {
 std::size_t particle_a{0};
 std::size_t particle_b{0};
 double rest_length{0.0};
 double stiffness{1.0};
 bool solve(std::vector<core::ParticleState>& particles) const noexcept;
};
}
