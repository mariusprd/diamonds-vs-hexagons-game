#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D_tema
{
    // Create square with given center, lenght = side_len and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 center, float length, glm::vec3 color, bool fill = false);

    // Create rectangle with given left_bottom_corner, x_side_len, y_side_len, color
    Mesh* CreateRectangle(const std::string &name, glm::vec3 left_bottom_corner, float x_len, float y_len, glm::vec3 color, bool fill = false);

    // Create diamond with given center, size, color
    Mesh* CreateDiamond(const std::string &name, glm::vec3 center, float size, glm::vec3 color, glm::vec3 tun_color);

    // Create hexagon with given center, radius, color
    Mesh* CreateHexagon(const std::string &name, glm::vec3 center, float radius, glm::vec3 color, glm::vec3 center_color);

    // Create star with given center, radius, color
    Mesh* CreateStar(const std::string &name, glm::vec3 center, float size, glm::vec3 color);

    // Create life with given center, size, color
    Mesh* CreateLife(const std::string &name, glm::vec3 center, float size, glm::vec3 color);
}
