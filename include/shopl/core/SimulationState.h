#pragma once

#include <cstddef>

namespace shopl::core {

// Minimal, solver-independent state for the initial integration milestone.
// This deliberately does not expose PositionBasedDynamics types yet.
struct SimulationState {
    double elapsed_seconds{0.0};
    std::size_t completed_steps{0};

    // Advances bookkeeping only; the actual solver is integrated behind a
    // separate adapter after the upstream build has been validated.
    bool record_step(double delta_seconds) noexcept;
};

} // namespace shopl::core
