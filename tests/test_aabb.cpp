#include "shopl/collision/Aabb.h"

#include <iostream>
#include <limits>

#define CHECK(condition) do { \
    if (!(condition)) { \
        std::cerr << "Check failed at line " << __LINE__ << ": " #condition "\n"; \
        return 1; \
    } \
} while (false)

using shopl::collision::Aabb;
using shopl::core::Vec3;

int main() {
    const Aabb unit{{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
    const Aabb separated{{1.01, 0.0, 0.0}, {2.0, 1.0, 1.0}};
    const Aabb touching_face{{1.0, 0.25, 0.25}, {2.0, 0.75, 0.75}};
    const Aabb contained{{0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}};
    const Aabb reversed{{1.0, 0.0, 0.0}, {0.0, 1.0, 1.0}};
    const Aabb non_finite{{0.0, 0.0, 0.0},
                          {1.0, 1.0, std::numeric_limits<double>::infinity()}};

    CHECK(unit.is_valid());
    CHECK(!reversed.is_valid());
    CHECK(!non_finite.is_valid());

    CHECK(unit.overlaps(unit));
    CHECK(unit.overlaps(touching_face));
    CHECK(touching_face.overlaps(unit));
    CHECK(unit.overlaps(contained));
    CHECK(!unit.overlaps(separated));
    CHECK(!unit.overlaps(reversed));

    CHECK(unit.contains({0.0, 0.0, 0.0}));
    CHECK(unit.contains({1.0, 1.0, 1.0}));
    CHECK(unit.contains({0.5, 0.5, 0.5}));
    CHECK(!unit.contains({1.1, 0.5, 0.5}));
    CHECK(!unit.contains({0.5, 0.5, 0.5 + 1.0e-9}));
    CHECK(!unit.contains({std::numeric_limits<double>::quiet_NaN(), 0.5, 0.5}));

    return 0;
}
