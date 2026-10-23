#include "shopl/core/SimulationState.h"

#include <cmath>
#include <limits>

namespace shopl::core {

bool SimulationState::record_step(const double delta_seconds) noexcept {
    if (!std::isfinite(delta_seconds) || delta_seconds <= 0.0) {
        return false;
    }

    if (elapsed_seconds > std::numeric_limits<double>::max() - delta_seconds) {
        return false;
    }

    elapsed_seconds += delta_seconds;
    ++completed_steps;
    return true;
}

} // namespace shopl::core
