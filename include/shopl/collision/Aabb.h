#pragma once

#include "shopl/core/Vec3.h"

namespace shopl::collision {

// Axis-aligned bounds used by broad-phase collision queries.
// Bounds are inclusive: boxes that touch at a face, edge, or point overlap.
struct Aabb {
    core::Vec3 min;
    core::Vec3 max;

    bool is_valid() const noexcept {
        return min.is_finite() && max.is_finite() &&
               min.x <= max.x && min.y <= max.y && min.z <= max.z;
    }

    bool overlaps(const Aabb& other) const noexcept {
        return is_valid() && other.is_valid() &&
               min.x <= other.max.x && max.x >= other.min.x &&
               min.y <= other.max.y && max.y >= other.min.y &&
               min.z <= other.max.z && max.z >= other.min.z;
    }

    bool contains(const core::Vec3& point) const noexcept {
        return is_valid() && point.is_finite() &&
               point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }
};

} // namespace shopl::collision
