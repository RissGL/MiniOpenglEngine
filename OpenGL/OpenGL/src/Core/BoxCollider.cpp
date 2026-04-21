#include "BoxCollider.h"

bool BoxCollider::IsOverlapping(BoxCollider* other) const
{
    if (other == this) return false; // 不能和自己撞

    glm::vec3 min1 = GetMin();
    glm::vec3 max1 = GetMax();
    glm::vec3 min2 = other->GetMin();
    glm::vec3 max2 = other->GetMax();

    return (min1.x <= max2.x && max1.x >= min2.x) &&
        (min1.y <= max2.y && max1.y >= min2.y) &&
        (min1.z <= max2.z && max1.z >= min2.z);
}
