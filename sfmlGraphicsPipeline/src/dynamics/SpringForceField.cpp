#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2, float stiffness, float equilibriumLength, float damping) :
    m_p1(p1),
    m_p2(p2),
    m_stiffness(stiffness), // stiffness k
    m_equilibriumLength(equilibriumLength), // equilibrium length L0
    m_damping(damping) //  medium viscosity coefficient c
{}

void SpringForceField::do_addForce()
{
    //TODO: Implement a damped spring
    //Functions to use:
    //glm::length( vec3 ): Return the length of a vector
    //glm::normalize( vec3 ): Return the normalization of a vector
    //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless

    //Compute displacement vector
    glm::vec3 displacement = getParticle1()->getPosition() - getParticle2()->getPosition();

    //Compute displacement length
    float length = glm::length(displacement);

    //Compute spring force corresponding to the displacement 
    //If the displacement is measurable by the computer (otherwise no force)

    if (length > std::numeric_limits<float>::epsilon()) {

        // Spring force :  −k * (||xi − xj|| − L0) * ((xi − xj) / ||xi − xj||)
        glm::vec3 springForce = -1.0f * (float) m_stiffness * (float) (length - m_equilibriumLength) * glm::normalize(displacement) ;

        // Damping : Fdamping​ = −c[(v1​−v2​)⋅d]d
        glm::vec3 deltaVelocity = getParticle1()->getVelocity() - getParticle2()->getVelocity();
        glm::vec3 dampingForce = -m_damping * glm::dot(deltaVelocity, glm::normalize(displacement)) * glm::normalize(displacement);

        // Total force
        glm::vec3 force = (springForce + dampingForce);

        getParticle1()->setForce(getParticle1()->getForce() + force);
        getParticle2()->setForce(getParticle2()->getForce() - force);
    }
}


ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
