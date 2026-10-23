#include "shopl/stepper/FixedStepStepper.h"
#include "shopl/constraints/DistanceConstraintSolver.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#define CHECK(condition) do { \
    if (!(condition)) { \
        std::cerr << "Check failed at line " << __LINE__ << ": " #condition "\n"; \
        return 1; \
    } \
} while (false)

using shopl::core::ParticleState;
using shopl::core::SimulationState;
using shopl::constraints::DistanceConstraint;
using shopl::constraints::DistanceConstraintSolver;
using shopl::stepper::FixedStepStepper;

int main() {
    // Compare the integrator against the analytical constant-acceleration
    // solution over one second at several fixed time steps. Semi-implicit
    // Euler has a known first-order position error of |g|*dt/2 here.
    for (const double dt : {1.0 / 30.0, 1.0 / 60.0, 1.0 / 120.0, 1.0 / 240.0}) {
        const auto steps = static_cast<std::size_t>(std::llround(1.0 / dt));
        std::vector<ParticleState> particles(1);
        SimulationState state;
        constexpr double gravity = -9.8;
        CHECK(FixedStepStepper::advance_steps(
            particles, {}, state, {0.0, gravity, 0.0}, dt, steps, 2));
        const double exact_position = 0.5 * gravity;
        const double expected_discrete = gravity * dt * dt *
            static_cast<double>(steps * (steps + 1)) / 2.0;
        CHECK(std::abs(particles[0].position.y - expected_discrete) < 1e-10);
        CHECK(std::abs(particles[0].position.y - exact_position) <=
              std::abs(gravity) * dt * 0.51);
        CHECK(particles[0].position.is_finite());
        CHECK(particles[0].velocity.is_finite());
    }

    // A pinned anchor and a constrained particle under gravity should remain
    // at the requested distance without stale velocity growing every frame.
    {
        std::vector<ParticleState> particles(2);
        particles[0].inverse_mass = 0.0;
        particles[1].position = {0.0, -2.0, 0.0};
        const std::vector<DistanceConstraint> constraints{{0, 1, 2.0, 1.0}};
        SimulationState state;
        CHECK(FixedStepStepper::advance_steps(
            particles, constraints, state, {0.0, -9.8, 0.0},
            1.0 / 120.0, 2400, 8));
        const auto delta = particles[1].position - particles[0].position;
        const double distance = std::sqrt(
            delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
        CHECK(std::abs(distance - 2.0) < 1e-8);
        CHECK(particles[0].position.is_finite());
        CHECK(particles[1].position.is_finite());
        CHECK(particles[1].velocity.is_finite());
        CHECK(std::sqrt(
            particles[1].velocity.x * particles[1].velocity.x +
            particles[1].velocity.y * particles[1].velocity.y +
            particles[1].velocity.z * particles[1].velocity.z) < 1e-5);
        CHECK(state.completed_steps == 2400);
    }


    // Sweep solver iteration counts on a small constraint chain. More iterations
    // should reduce the maximum rest-length error without producing non-finite
    // coordinates. This measures convergence instead of merely checking success.
    {
        const std::vector<DistanceConstraint> chain{{0, 1, 2.0, 1.0},
                                                     {1, 2, 2.0, 1.0}};
        auto maximum_error = [&](std::size_t iterations) {
            std::vector<ParticleState> particles(3);
            particles[1].position = {3.0, 0.0, 0.0};
            particles[2].position = {6.0, 0.0, 0.0};
            if (!DistanceConstraintSolver::solve(particles, chain, iterations)) return 1e100;
            const double first = std::sqrt(
                std::pow(particles[1].position.x - particles[0].position.x, 2.0) +
                std::pow(particles[1].position.y - particles[0].position.y, 2.0) +
                std::pow(particles[1].position.z - particles[0].position.z, 2.0));
            const double second = std::sqrt(
                std::pow(particles[2].position.x - particles[1].position.x, 2.0) +
                std::pow(particles[2].position.y - particles[1].position.y, 2.0) +
                std::pow(particles[2].position.z - particles[1].position.z, 2.0));
            for (const auto& particle : particles) {
                if (!particle.position.is_finite()) return 1e100;
            }
            return std::max(std::abs(first - 2.0), std::abs(second - 2.0));
        };
        const double one_iteration_error = maximum_error(1);
        const double eight_iteration_error = maximum_error(8);
        const double thirty_two_iteration_error = maximum_error(32);
        CHECK(one_iteration_error > 1e-3);
        CHECK(eight_iteration_error < one_iteration_error);
        CHECK(thirty_two_iteration_error <= eight_iteration_error);
        CHECK(thirty_two_iteration_error < 1e-6);
    }

    return 0;
}
