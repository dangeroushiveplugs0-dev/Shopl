#include "shopl/constraints/DistanceConstraintSolver.h"
namespace shopl::constraints {
bool DistanceConstraintSolver::solve(std::vector<core::ParticleState>& particles,
 const std::vector<DistanceConstraint>& constraints, const std::size_t iterations) {
 if (iterations == 0) return false;
 auto working = particles;
 for (std::size_t pass = 0; pass < iterations; ++pass) {
  for (const auto& constraint : constraints) {
   if (!constraint.solve(working)) return false;
  }
 }
 particles.swap(working);
 return true;
}
}
