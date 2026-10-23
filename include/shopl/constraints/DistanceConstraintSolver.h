#pragma once
#include <cstddef>
#include <vector>
#include "shopl/constraints/DistanceConstraint.h"
namespace shopl::constraints {
class DistanceConstraintSolver {
public:
 // Runs constraints in input order for each iteration. Failure leaves input unchanged.
 static bool solve(std::vector<core::ParticleState>& particles,
                   const std::vector<DistanceConstraint>& constraints,
                   std::size_t iterations);
};
}
