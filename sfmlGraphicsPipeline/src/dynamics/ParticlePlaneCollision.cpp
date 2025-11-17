#include "./../../include/dynamics/ParticlePlaneCollision.hpp"

ParticlePlaneCollision::~ParticlePlaneCollision()
{}


ParticlePlaneCollision::ParticlePlaneCollision(ParticlePtr particle, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_particle = particle;
    m_plane = plane;
}

void ParticlePlaneCollision::do_solveCollision()
{
    //Don't process fixed particles (Let's assume that the ground plane is fixed)
    if (m_particle->isFixed()) return;

    //TODO: Solve ParticlePlane collisions, update particle position and velocity after collision
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition(), Particle::getVelocity(), Particle::setPosition(), Particle::setVelocity()

    //Compute particle-plane distance 
    // distance = dot(P, n) + d
    float particlePlaneDist = glm::dot(m_particle->getPosition(), m_plane->normal()) + m_plane->distanceToOrigin();

    //Compute plane-particle vector
    glm::vec3 normal = glm::normalize(m_plane->normal());

    // Compute post-collision velocity
    glm::vec3 velocity = m_particle->getVelocity();

    // Decompose the velocity into normal and tangential parts
    float vN = glm::dot(velocity, normal);
    glm::vec3 vNormal = vN * normal;
    glm::vec3 vTangent = velocity - vNormal;

    // If the particle is moving toward the plane (vN < 0), reflect it
    if (vN < 0.0f)
    {
        // Place the particle right above the plane, so that it doesn't trigger another collision at the nexte frame
        glm::vec3 correctedPos = m_particle->getPosition() + (m_particle->getRadius() - particlePlaneDist) * normal;
        m_particle->setPosition(correctedPos);

        glm::vec3 newVelocity = vTangent - m_restitution * vNormal;
        m_particle->setVelocity(newVelocity);
    }
}


bool testParticlePlane(const ParticlePtr &particle, const PlanePtr &plane)
{
    /* Equation of a plane passing through A and normal to n:
   * dot( p - A, n ) = dot( p, n ) - dot( A, n ) = 0
   * dot( A, n ) is stored in the "distanceToOrigin" of the plane.
   *
   * Equation of a particle of radius r centered in c:
   * dot( p - c, p - c ) = r²
   *
   * distance( plane, particle )
   *   = min( distance( plane, c ) - r, 0 )              //definition
   *   = min( abs( dot( c - A, n ) ) - r, 0 )
   *   = min( abs( dot( c, n ) - dot( A, n ) ) - r, 0 )
   *
   * So, there is intersection if distance( plane, particle ) = 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) - r <= 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) <= r
   */

    //TODO: Test collision between particle and plane
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition()

    float particlePlaneDist = glm::dot(particle->getPosition(), plane->normal()) + plane->distanceToOrigin();

    // By doing this, the particle still clip a bit through the surface
    return particlePlaneDist <= particle->getRadius() ;
}
