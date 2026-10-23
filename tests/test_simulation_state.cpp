#include "shopl/core/SimulationState.h"

#include <cmath>
#include <iostream>
#include <limits>

#define CHECK(condition) do { \
    if (!(condition)) { \
        std::cerr << "Check failed at line " << __LINE__ << ": " #condition "\n"; \
        return 1; \
    } \
} while (false)

int main() {
    shopl::core::SimulationState state;

    CHECK(state.completed_steps == 0);
    CHECK(state.elapsed_seconds == 0.0);

    CHECK(state.record_step(1.0 / 60.0));
    CHECK(state.completed_steps == 1);
    CHECK(std::abs(state.elapsed_seconds - (1.0 / 60.0)) < 1e-12);

    CHECK(!state.record_step(0.0));
    CHECK(!state.record_step(-0.1));
    CHECK(!state.record_step(std::numeric_limits<double>::infinity()));
    CHECK(!state.record_step(std::numeric_limits<double>::quiet_NaN()));
    CHECK(state.completed_steps == 1);

    return 0;
}
