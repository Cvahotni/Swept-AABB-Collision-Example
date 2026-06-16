#include "pch.h"
#include "math/vector3.h"

void Vector3::Cross(Vector3& vec1, Vector3& vec2) {
    vector3 = glm::cross(vec1.Raw(), vec2.Raw());
}

void Vector3::Normalize() {
    vector3 = glm::normalize(vector3);
}
