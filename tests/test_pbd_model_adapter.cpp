#include "shopl/pbd/PbdModelAdapter.h"

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
    using shopl::core::Vec3;
    using shopl::pbd::PbdModelAdapter;

    PbdModelAdapter adapter;
    const auto first = adapter.add_particle({0.0, 0.0, 0.0}, 1.0);
    const auto second = adapter.add_particle({1.0, 0.0, 0.0}, 2.0);
    CHECK(first.has_value() && *first == 0);
    CHECK(second.has_value() && *second == 1);
    CHECK(adapter.particle_count() == 2);

    const auto first_position = adapter.particle_position(*first);
    CHECK(first_position.has_value());
    CHECK(first_position->x == 0.0 && first_position->y == 0.0);

    CHECK(adapter.set_particle_position(*second, {1.5, 2.0, -3.0}));
    const auto updated = adapter.particle_position(*second);
    CHECK(updated.has_value());
    CHECK(updated->x == 1.5 && updated->y == 2.0 && updated->z == -3.0);

    CHECK(adapter.add_distance_constraint(*first, *second, 0.8));
    CHECK(!adapter.add_distance_constraint(*first, *first, 0.8));
    CHECK(!adapter.add_distance_constraint(*first, 99, 0.8));
    CHECK(!adapter.add_distance_constraint(*first, *second, 1.1));
    CHECK(!adapter.add_distance_constraint(*first, *second,
                                           std::numeric_limits<double>::quiet_NaN()));

    CHECK(!adapter.add_particle({std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0}));
    CHECK(!adapter.add_particle({0.0, 0.0, 0.0}, -1.0));
    CHECK(!adapter.particle_position(99).has_value());
    CHECK(!adapter.set_particle_position(99, Vec3{}));
    CHECK(!adapter.set_particle_position(*first, {0.0,
        std::numeric_limits<double>::infinity(), 0.0}));

    return 0;
}
