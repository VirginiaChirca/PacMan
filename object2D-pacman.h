#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    Mesh* CreateCircle(const std::string& name, float numTriangle, float radiusX, float radiusY, glm::vec3 color);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
    Mesh* Heart(std::string name, glm::vec3 center, float radius, glm::vec3 color);
}
