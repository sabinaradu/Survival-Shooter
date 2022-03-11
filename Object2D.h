#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Object2D
{
    Mesh* CreateRectangle(const std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color);
    Mesh* CreateCircle(std::string name, float nr_tring, float radius, glm::vec3 color);
}   // namespace Object2D