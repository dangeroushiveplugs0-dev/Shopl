#pragma once

#include <cstddef>
#include <vector>

#include "shopl/constraints/DistanceConstraint.h"
#include "shopl/core/SimulationState.h"
#include "shopl/core/Vec3.h"

namespace shopl::stepper {

// Advances a requested number of equal-duration simulation steps.
// The operation is transactional: particles and bookkeeping are committed
// only if every integration and constraint solve succeeds.
class FixedStepStepper {
public:
    static bool advance_steps(
        std::vector<core::ParticleState>& particles,
        const std::vector<constraints::DistanceConstraint>& constraints,
        core::SimulationState& simulation,
        const core::Vec3& acceleration,
        double fixed_delta_seconds,
        std::size_t step_count,
        std::size_t constraint_iterations);
};

} // namespace shopl::stepper
