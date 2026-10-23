#include "shopl/constraints/DistanceConstraint.h"
#include <cmath>
namespace shopl::constraints {
bool DistanceConstraint::solve(std::vector<core::ParticleState>& p) const noexcept {
 if (particle_a>=p.size() || particle_b>=p.size() || particle_a==particle_b || !std::isfinite(rest_length) || rest_length<0.0 || !std::isfinite(stiffness) || stiffness<0.0 || stiffness>1.0) return false;
 auto& a=p[particle_a]; auto& b=p[particle_b];
 if (!a.position.is_finite() || !b.position.is_finite() || !std::isfinite(a.inverse_mass) || !std::isfinite(b.inverse_mass) || a.inverse_mass<0.0 || b.inverse_mass<0.0) return false;
 if (stiffness==0.0) return true;
 const double weights=a.inverse_mass+b.inverse_mass;
 if (!std::isfinite(weights)) return false;
 if (weights==0.0) return true;
 const core::Vec3 d=b.position-a.position;
 const double q=d.x*d.x+d.y*d.y+d.z*d.z;
 if (!std::isfinite(q)) return false;
 if (q==0.0) return rest_length==0.0;
 const double len=std::sqrt(q);
 const double scale=stiffness*(len-rest_length)/(len*weights);
 if (!std::isfinite(scale)) return false;
 const core::Vec3 c=d*scale;
 const core::Vec3 na=a.position+c*a.inverse_mass, nb=b.position-c*b.inverse_mass;
 if (!na.is_finite() || !nb.is_finite()) return false;
 a.position=na; b.position=nb; return true;
}
}
