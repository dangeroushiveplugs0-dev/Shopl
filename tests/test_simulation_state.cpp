#include "shopl/core/SimulationState.h"

#include <cassert>
#include <cmath>
#include <limits>

int main() {
    shopl::core::SimulationState state;

    assert(state.completed_steps == 0);
    assert(state.elapsed_seconds == 0.0);

    assert(state.record_step(1.0 / 60.0));
    assert(state.completed_steps == 1);
    assert(std::abs(state.elapsed_seconds - (1.0 / 60.0)) < 1e-12);

    assert(!state.record_step(0.0));
    assert(!state.record_step(-0.1));
    assert(!state.record_step(std::numeric_limits<double>::infinity()));
    assert(!state.record_step(std::numeric_limits<double>::quiet_NaN()));
    assert(state.completed_steps == 1);

    return 0;
}
