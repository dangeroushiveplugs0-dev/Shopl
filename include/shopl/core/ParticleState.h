#pragma once
#include "shopl/core/Vec3.h"
namespace shopl::core {
// An inverse mass of zero marks a fixed/pinned particle.
struct ParticleState { Vec3 position{}; Vec3 velocity{}; double inverse_mass{1.0}; };
}
