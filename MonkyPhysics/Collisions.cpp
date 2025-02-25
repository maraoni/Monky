#include "Collisions.h"

glm::mat3 Banana::ComputeMomentOfInertiaBox(float mass, glm::vec3 extents, glm::vec3 aPosition)
{
    float w2 = extents.x * extents.x;
    float h2 = extents.y * extents.y;
    float d2 = extents.z * extents.z;
    
    glm::mat3 inertiaTensor = glm::mat3(0.0f);
    inertiaTensor[0][0] = (1.0f / 12.0f) * mass * (h2 + d2);
    inertiaTensor[1][1] = (1.0f / 12.0f) * mass * (w2 + d2);
    inertiaTensor[2][2] = (1.0f / 12.0f) * mass * (w2 + h2);
    
    // Parallel Axis Theorem
    glm::mat3 parallelAxis = mass * glm::mat3(
        aPosition.y * aPosition.y + aPosition.z * aPosition.z, -aPosition.x * aPosition.y, -aPosition.x * aPosition.z,
        -aPosition.x * aPosition.y, aPosition.x * aPosition.x + aPosition.z * aPosition.z, -aPosition.y * aPosition.z,
        -aPosition.x * aPosition.z, -aPosition.y * aPosition.z, aPosition.x * aPosition.x + aPosition.y * aPosition.y
    );
    
    inertiaTensor += parallelAxis;
    
    return inertiaTensor;
}

glm::mat3 Banana::ComputeMomentOfInertiaSphere(float mass, float radius)
{
	float inertiaScalar = (2.0f / 3.0f) * mass * (radius * radius);
	return glm::mat3(inertiaScalar);
}
