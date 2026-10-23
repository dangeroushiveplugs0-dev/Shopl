#include "shopl/stepper/FixedStepStepper.h"

#include <cmath>
#include <limits>
#include <utility>

#include "shopl/constraints/DistanceConstraintSolver.h"
#include "shopl/core/GravityIntegrator.h"

namespace shopl::stepper {

bool FixedStepStepper::advance_steps(
    std::vector<core::ParticleState>& particles,
    const std::vector<constraints::DistanceConstraint>& constraints,
    core::SimulationState& simulation,
    const core::Vec3& acceleration,
    const double fixed_delta_seconds,
    const std::size_t step_count,
    const std::size_t constraint_iterations) {
    if (step_count == 0 || constraint_iterations == 0 ||
        !std::isfinite(fixed_delta_seconds) || fixed_delta_seconds <= 0.0 ||
        !acceleration.is_finite() ||
        !std::isfinite(simulation.elapsed_seconds) ||
        simulation.elapsed_seconds < 0.0 ||
        step_count > std::numeric_limits<std::size_t>::max() -
                         simulation.completed_steps ||
        fixed_delta_seconds >
            (std::numeric_limits<double>::max() - simulation.elapsed_seconds) /
                static_cast<double>(step_count)) {
        return false;
    }

    std::vector<core::ParticleState> working_particles = particles;
    core::SimulationState working_simulation = simulation;

    for (std::size_t step = 0; step < step_count; ++step) {
        for (core::ParticleState& particle : working_particles) {
            if (!particle.position.is_finite() || !particle.velocity.is_finite() ||
                !std::isfinite(particle.inverse_mass) ||
                particle.inverse_mass < 0.0) {
                return false;
            }

            // A zero inverse mass marks a pinned particle. Leave both its
            // position and velocity unchanged during gravity integration.
            if (particle.inverse_mass == 0.0) {
                continue;
            }

            if (!core::GravityIntegrator::step(
                    particle, acceleration, fixed_delta_seconds)) {
                return false;
            }
        }

        if (!constraints::DistanceConstraintSolver::solve(
                working_particles, constraints, constraint_iterations) ||
            !working_simulation.record_step(fixed_delta_seconds)) {
            return false;
        }
    }

    particles.swap(working_particles);
    simulation = working_simulation;
    return true;
}

} // namespace shopl::stepper
