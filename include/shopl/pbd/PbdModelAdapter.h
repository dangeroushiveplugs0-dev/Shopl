#pragma once

#include "shopl/core/Vec3.h"

#include <Simulation/SimulationModel.h>

#include <cstddef>
#include <optional>

namespace shopl::pbd {

// Owns the upstream model while keeping Shopl-facing data in solver-neutral
// types. This adapter creates particles and constraints; it does not advance
// the solver yet. A TimeStep bridge is a separate, testable milestone.
class PbdModelAdapter {
public:
    PbdModelAdapter();

    PbdModelAdapter(const PbdModelAdapter&) = delete;
    PbdModelAdapter& operator=(const PbdModelAdapter&) = delete;

    std::optional<std::size_t> add_particle(const core::Vec3& position,
                                            double mass = 1.0) noexcept;
    std::optional<core::Vec3> particle_position(std::size_t index) const noexcept;
    bool set_particle_position(std::size_t index,
                               const core::Vec3& position) noexcept;
    bool add_distance_constraint(std::size_t first,
                                 std::size_t second,
                                 double stiffness) noexcept;

    std::size_t particle_count() const noexcept;

    // Deliberately exposed only at the adapter boundary for the future
    // TimeStep bridge and advanced integrations.
    PBD::SimulationModel& native_model() noexcept { return model_; }
    const PBD::SimulationModel& native_model() const noexcept { return model_; }

private:
    PBD::SimulationModel model_;
};

} // namespace shopl::pbd
