#include "shopl/stepper/FixedStepStepper.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#define CHECK(condition) do { \
    if (!(condition)) { \
        std::cerr << "Check failed at line " << __LINE__ << ": " #condition "\n"; \
        return 1; \
    } \
} while (false)

using shopl::core::ParticleState;
using shopl::core::SimulationState;
using shopl::core::Vec3;
using shopl::constraints::DistanceConstraint;
using shopl::stepper::FixedStepStepper;

int main() {
    // Semi-implicit Euler has a known discrete result under constant gravity:
    // v_n = a*n*dt; y_n = y_0 + a*dt^2*n*(n+1)/2.
    {
        std::vector<ParticleState> particles(1);
        SimulationState simulation;
        constexpr std::size_t steps = 100;
        constexpr double dt = 0.01;
        constexpr double gravity = -9.8;
        CHECK(FixedStepStepper::advance_steps(
            particles, {}, simulation, {0.0, gravity, 0.0}, dt, steps, 4));
        CHECK(std::abs(particles[0].velocity.y - gravity * steps * dt) < 1e-12);
        const double expected_y = gravity * dt * dt *
            static_cast<double>(steps * (steps + 1)) / 2.0;
        CHECK(std::abs(particles[0].position.y - expected_y) < 1e-11);
        CHECK(simulation.completed_steps == steps);
        CHECK(std::abs(simulation.elapsed_seconds - steps * dt) < 1e-12);
    }

    // Repeating the same scenario from the same initial state is bit-repeatable.
    {
        std::vector<ParticleState> first(2);
        first[1].position = {2.0, 3.0, 0.0};
        std::vector<ParticleState> second = first;
        SimulationState first_state;
        SimulationState second_state;
        const std::vector<DistanceConstraint> constraints{{0, 1, 2.0, 0.75}};
        CHECK(FixedStepStepper::advance_steps(
            first, constraints, first_state, {0.0, -9.8, 0.0}, 1.0 / 120.0, 60, 6));
        CHECK(FixedStepStepper::advance_steps(
            second, constraints, second_state, {0.0, -9.8, 0.0}, 1.0 / 120.0, 60, 6));
        CHECK(first.size() == second.size());
        for (std::size_t i = 0; i < first.size(); ++i) {
            CHECK(first[i].position.x == second[i].position.x);
            CHECK(first[i].position.y == second[i].position.y);
            CHECK(first[i].position.z == second[i].position.z);
            CHECK(first[i].velocity.x == second[i].velocity.x);
            CHECK(first[i].velocity.y == second[i].velocity.y);
            CHECK(first[i].velocity.z == second[i].velocity.z);
        }
        CHECK(first_state.elapsed_seconds == second_state.elapsed_seconds);
        CHECK(first_state.completed_steps == second_state.completed_steps);
    }

    // Pinned particles do not drift under gravity.
    {
        std::vector<ParticleState> particles(1);
        particles[0].position = {1.0, 2.0, 3.0};
        particles[0].velocity = {4.0, 5.0, 6.0};
        particles[0].inverse_mass = 0.0;
        SimulationState simulation;
        CHECK(FixedStepStepper::advance_steps(
            particles, {}, simulation, {0.0, -9.8, 0.0}, 0.01, 5, 2));
        CHECK(particles[0].position.x == 1.0);
        CHECK(particles[0].position.y == 2.0);
        CHECK(particles[0].position.z == 3.0);
        CHECK(particles[0].velocity.x == 4.0);
        CHECK(particles[0].velocity.y == 5.0);
        CHECK(particles[0].velocity.z == 6.0);
    }

    // A late invalid constraint must not partially commit earlier steps.
    {
        std::vector<ParticleState> particles(2);
        particles[1].position = {4.0, 0.0, 0.0};
        const auto before = particles;
        SimulationState simulation{1.0, 7};
        const auto before_state = simulation;
        const std::vector<DistanceConstraint> constraints{
            {0, 1, 2.0, 1.0}, {0, 9, 1.0, 1.0}};
        CHECK(!FixedStepStepper::advance_steps(
            particles, constraints, simulation, {0.0, -9.8, 0.0}, 0.01, 3, 2));
        CHECK(particles[0].position.x == before[0].position.x);
        CHECK(particles[1].position.x == before[1].position.x);
        CHECK(particles[1].position.y == before[1].position.y);
        CHECK(particles[0].velocity.y == before[0].velocity.y);
        CHECK(simulation.elapsed_seconds == before_state.elapsed_seconds);
        CHECK(simulation.completed_steps == before_state.completed_steps);
    }

    // Invalid scheduling parameters fail without changing state.
    {
        std::vector<ParticleState> particles(1);
        SimulationState simulation;
        CHECK(!FixedStepStepper::advance_steps(
            particles, {}, simulation, {0.0, -9.8, 0.0}, 0.01, 0, 2));
        CHECK(!FixedStepStepper::advance_steps(
            particles, {}, simulation, {0.0, -9.8, 0.0}, 0.01, 1, 0));
        CHECK(!FixedStepStepper::advance_steps(
            particles, {}, simulation, {0.0, -9.8, 0.0},
            std::numeric_limits<double>::infinity(), 1, 2));
        CHECK(simulation.completed_steps == 0);
        CHECK(simulation.elapsed_seconds == 0.0);
        CHECK(particles[0].position.y == 0.0);
    }

    return 0;
}
