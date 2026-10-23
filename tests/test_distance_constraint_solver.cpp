#include "shopl/constraints/DistanceConstraintSolver.h"
#include <cmath>
#include <iostream>
#include <vector>
#define CHECK(x) do { if (!(x)) { std::cerr << "Check failed line " << __LINE__ << ": " #x << '\n'; return 1; } } while(false)
using shopl::core::ParticleState; using shopl::constraints::DistanceConstraint; using shopl::constraints::DistanceConstraintSolver;
static double dist(double a,double b){return std::abs(b-a);}
int main(){
 {std::vector<ParticleState> p(3);p[1].position={3,0,0};p[2].position={6,0,0};std::vector<DistanceConstraint> c{{0,1,2,1},{1,2,2,1}};CHECK(DistanceConstraintSolver::solve(p,c,40));CHECK(std::abs(dist(p[0].position.x,p[1].position.x)-2)<1e-6);CHECK(std::abs(dist(p[1].position.x,p[2].position.x)-2)<1e-6);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};std::vector<DistanceConstraint> c{{0,1,2,1},{0,8,1,1}};CHECK(!DistanceConstraintSolver::solve(p,c,2));CHECK(p[0].position.x==0);CHECK(p[1].position.x==4);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};std::vector<DistanceConstraint> c{{0,1,2,1}};CHECK(!DistanceConstraintSolver::solve(p,c,0));CHECK(p[1].position.x==4);}
 {std::vector<ParticleState> p(1);std::vector<DistanceConstraint> c;CHECK(DistanceConstraintSolver::solve(p,c,3));CHECK(p[0].position.x==0);}
 {std::vector<ParticleState> p(2);p[1].position={4,0,0};std::vector<DistanceConstraint> c{{0,1,2,.5}};auto first=p;CHECK(DistanceConstraintSolver::solve(p,c,4));auto second=first;CHECK(DistanceConstraintSolver::solve(second,c,4));CHECK(p[0].position.x==second[0].position.x);CHECK(p[1].position.x==second[1].position.x);}
 return 0;
}
