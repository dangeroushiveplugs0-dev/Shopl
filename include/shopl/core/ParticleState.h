#pragma once

#include "shopl/core/Vec3.h"

namespace shopl::core {

// Minimal particle data shared by solver adapters and headless tests.
struct ParticleState {
    Vec3 position{};
    Vec3 velocity{};
};

} // namespace shopl::core
