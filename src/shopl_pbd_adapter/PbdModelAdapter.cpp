#include "shopl/pbd/PbdModelAdapter.h"

#include <cmath>
#include <limits>

namespace shopl::pbd {

PbdModelAdapter::PbdModelAdapter() {
    model_.init();
}

std::optional<std::size_t> PbdModelAdapter::add_particle(
    const core::Vec3& position, const double mass) noexcept {
    if (!position.is_finite() || !std::isfinite(mass) || mass < 0.0) {
        return std::nullopt;
    }

    auto& particles = model_.getParticles();
    const std::size_t count = particles.getNumberOfParticles();
    if (count >= static_cast<std::size_t>(std::numeric_limits<unsigned int>::max())) {
        return std::nullopt;
    }

    particles.addVertex(PBD::Vector3r(position.x, position.y, position.z));
    particles.setMass(static_cast<unsigned int>(count), mass);
    return count;
}

std::optional<core::Vec3> PbdModelAdapter::particle_position(
    const std::size_t index) const noexcept {
    const auto& particles = model_.getParticles();
    if (index >= particles.getNumberOfParticles()) {
        return std::nullopt;
    }

    const auto& position = particles.getPosition(static_cast<unsigned int>(index));
    core::Vec3 result{position.x(), position.y(), position.z()};
    if (!result.is_finite()) {
        return std::nullopt;
    }
    return result;
}

bool PbdModelAdapter::set_particle_position(
    const std::size_t index, const core::Vec3& position) noexcept {
    if (!position.is_finite()) {
        return false;
    }

    auto& particles = model_.getParticles();
    if (index >= particles.getNumberOfParticles()) {
        return false;
    }

    particles.setPosition(static_cast<unsigned int>(index),
                          PBD::Vector3r(position.x, position.y, position.z));
    return true;
}

bool PbdModelAdapter::add_distance_constraint(
    const std::size_t first, const std::size_t second,
    const double stiffness) noexcept {
    const auto count = particle_count();
    if (first >= count || second >= count || first == second ||
        !std::isfinite(stiffness) || stiffness < 0.0 || stiffness > 1.0) {
        return false;
    }

    return model_.addDistanceConstraint(static_cast<unsigned int>(first),
                                        static_cast<unsigned int>(second),
                                        static_cast<PBD::Real>(stiffness));
}

std::size_t PbdModelAdapter::particle_count() const noexcept {
    return model_.getParticles().getNumberOfParticles();
}

} // namespace shopl::pbd
