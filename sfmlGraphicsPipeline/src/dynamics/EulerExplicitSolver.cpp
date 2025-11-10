#include "./../../include/dynamics/EulerExplicitSolver.hpp"
#define GRAVITY = 9.806 65 // Constant for gravity

EulerExplicitSolver::EulerExplicitSolver()
{

}

EulerExplicitSolver::~EulerExplicitSolver()
{

}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<ParticlePtr>& particles)
{
    for(ParticlePtr p : particles)
    {
        if(!p->isFixed())
        {
            //TODO: Implement explicit euler solver
            //Functions to use:
            //Particle::getPosition(), Particle::getVelocity(), Particle::getMass(), Particle::getForce()
            //Particle::setPosition(), Particle::setVelocity()

            p->setVelocity(p->getVelocity() + (1/p->getMass()) * p->getForce() * dt);
            p->setPosition(p->getPosition() + p->getVelocity() * dt);
        }
    }
}
