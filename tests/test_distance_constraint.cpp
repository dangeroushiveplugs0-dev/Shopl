#include "shopl/constraints/DistanceConstraint.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#define CHECK(x) do { if (!(x)) { std::cerr << "Check failed line " << __LINE__ << ": " #x "\\n"; return 1; } } while(false)
using shopl::core::ParticleState; using shopl::core::Vec3; using shopl::constraints::DistanceConstraint;
static double dist(const Vec3& a,const Vec3& b){const auto d=b-a;return std::sqrt(d.x*d.x+d.y*d.y+d.z*d.z);}
int main(){
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};DistanceConstraint c{0,1,2,1};CHECK(c.solve(p));CHECK(std::abs(dist(p[0].position,p[1].position)-2)<1e-12);CHECK(std::abs(p[0].position.x-1)<1e-12);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};p[0].inverse_mass=0;DistanceConstraint c{0,1,2,1};CHECK(c.solve(p));CHECK(p[0].position.x==0);CHECK(std::abs(p[1].position.x-2)<1e-12);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};DistanceConstraint c{0,1,2,.25};CHECK(c.solve(p));CHECK(std::abs(dist(p[0].position,p[1].position)-3.5)<1e-12);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};p[0].inverse_mass=0;p[1].inverse_mass=0;DistanceConstraint c{0,1,2,1};CHECK(c.solve(p));CHECK(p[1].position.x==4);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};DistanceConstraint bad_index{0,2,2,1},same{0,0,2,1},negative_rest{0,1,-1,1},low{0,1,2,-.1},high{0,1,2,1.1},infinite{0,1,std::numeric_limits<double>::infinity(),1};CHECK(!bad_index.solve(p));CHECK(!same.solve(p));CHECK(!negative_rest.solve(p));CHECK(!low.solve(p));CHECK(!high.solve(p));CHECK(!infinite.solve(p));CHECK(p[1].position.x==4);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};DistanceConstraint c{0,1,2,1};p[0].inverse_mass=-1;CHECK(!c.solve(p));p[0].inverse_mass=std::numeric_limits<double>::quiet_NaN();CHECK(!c.solve(p));}
 {std::vector<ParticleState> p(2);DistanceConstraint zero{0,1,0,1},nonzero{0,1,1,1};CHECK(zero.solve(p));CHECK(!nonzero.solve(p));}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};DistanceConstraint c{0,1,2,0};CHECK(c.solve(p));CHECK(p[1].position.x==4);}
 return 0;
}
