#pragma once

#include <cmath>

namespace shopl::core {

// Small solver-independent vector type for public Shopl APIs.
struct Vec3 {
    double x{0.0};
    double y{0.0};
    double z{0.0};

    constexpr Vec3 operator+(const Vec3& other) const noexcept {
        return {x + other.x, y + other.y, z + other.z};
    }

    constexpr Vec3 operator-(const Vec3& other) const noexcept {
        return {x - other.x, y - other.y, z - other.z};
    }

    constexpr Vec3 operator*(double scalar) const noexcept {
        return {x * scalar, y * scalar, z * scalar};
    }

    constexpr Vec3& operator+=(const Vec3& other) noexcept {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    bool is_finite() const noexcept {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
    }
};

constexpr Vec3 operator*(double scalar, const Vec3& vector) noexcept {
    return vector * scalar;
}

} // namespace shopl::core
