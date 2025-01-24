#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D_tema::CreateSquare(
    const std::string &name,
    glm::vec3 center,
    float length,
    glm::vec3 color,
    bool fill
)
{
    float half_len = length / 2;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(-half_len, -half_len, 0), color),
        VertexFormat(center + glm::vec3(half_len, -half_len, 0), color),
        VertexFormat(center + glm::vec3(half_len, half_len, 0), color),
        VertexFormat(center + glm::vec3(-half_len, half_len, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D_tema::CreateRectangle(
    const std::string &name,
    glm::vec3 left_bottom_corner,
    float x_len,
    float y_len,
    glm::vec3 color,
    bool fill
)
{
    glm::vec3 corner = left_bottom_corner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(x_len, 0, 0), color),
        VertexFormat(corner + glm::vec3(x_len, y_len, 0), color),
        VertexFormat(corner + glm::vec3(0, y_len, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D_tema::CreateDiamond(
    const std::string &name,
    glm::vec3 center,
    float size,
    glm::vec3 color,
    glm::vec3 tun_color
)
{
    float vertical = 2;
    float turret_y = 0.5;
    float turret_x = 2;
    std::vector<VertexFormat> vertices = 
    {
        VertexFormat(center + glm::vec3(-size, 0, 0), tun_color),
        VertexFormat(center + glm::vec3(0, -size * vertical, 0), color),
        VertexFormat(center + glm::vec3(size, 0, 0), color),
        VertexFormat(center + glm::vec3(0, size * vertical, 0), color),
        VertexFormat(center + glm::vec3(0, -size * turret_y, 0), color),
        VertexFormat(center + glm::vec3(size * turret_x, -size * turret_y, 0), color),
        VertexFormat(center + glm::vec3(size * turret_x, size * turret_y, 0), color),
        VertexFormat(center + glm::vec3(0, size * turret_y, 0), color)
    };

    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices =
    { 
        0, 1, 2,
        3, 0, 2,
        4, 5, 6,
        7, 4, 6
    };

    diamond->InitFromData(vertices, indices);
    return diamond;
}

Mesh* object2D_tema::CreateHexagon(
    const std::string &name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    glm::vec3 center_color
)
{
    float tri_height = radius * sqrt(3) / 2;
    std::vector<VertexFormat> vertices = 
    {
        VertexFormat(center + glm::vec3(-radius, 0, 0), color),
        VertexFormat(center + glm::vec3(-radius / 2, -tri_height, 0), color),
        VertexFormat(center + glm::vec3(radius / 2, -tri_height, 0), color),
        VertexFormat(center + glm::vec3(radius, 0, 0), color),
        VertexFormat(center + glm::vec3(radius / 2, tri_height, 0), color),
        VertexFormat(center + glm::vec3(-radius / 2, tri_height, 0), color),
        VertexFormat(center + glm::vec3(0, 0, 0), center_color)
    };

    Mesh* hexagon = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        0, 1, 6,
        2, 6, 1,
        3, 6, 2,
        4, 6, 3,
        5, 6, 4,
        0, 6, 5
    };

    hexagon->InitFromData(vertices, indices);
    return hexagon;
}

Mesh* object2D_tema::CreateStar(
    const std::string &name,
    glm::vec3 center,
    float size,
    glm::vec3 color
)
{
    float to_rad = M_PI / 180;

    // pentagon height and radius
    float H = sqrt(5 + 2 * sqrt(5)) / 2 * size;
    float R = sqrt((5 + sqrt(5)) / 10) * size;

    // p1 -> p2 dist => ext triangle side
    float A = size / (2 * cos(72 * to_rad));
    // ext triangle height
    float B = size * tan(72 * to_rad) / 2;
    // short side of big triangle
    float C = H - R + B;
    // pos of p3, p4
    float X = C * sin(36 * to_rad);
    float Y = C * cos(36 * to_rad);
    
    std::vector<VertexFormat> vertices = 
    {
        VertexFormat(center + glm::vec3(-size / 2, H - R, 0), color),
        VertexFormat(center + glm::vec3(size / 2, H - R, 0), color),
        VertexFormat(center + glm::vec3(size / 2 + A, H - R, 0), color),
        VertexFormat(center + glm::vec3(X, -Y, 0), color),
        VertexFormat(center + glm::vec3(-X, -Y, 0), color),
        VertexFormat(center + glm::vec3(-size / 2 - A, H - R, 0), color),
        VertexFormat(center + glm::vec3(0, H - R + B, 0), color)
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices =
    {
        5, 3, 1,
        0, 4, 2,
        6, 0, 1
    };

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D_tema::CreateLife(
    const std::string &name,
    glm::vec3 center,
    float size,
    glm::vec3 color
) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(center + glm::vec3(-size, 0, 0), color),
        VertexFormat(center + glm::vec3(-size / 2, -size / 2, 0), color),
        VertexFormat(center + glm::vec3(0, -size, 0), color),
        VertexFormat(center + glm::vec3(size / 2, -size / 2, 0), color),
        VertexFormat(center + glm::vec3(size, 0, 0), color),
        VertexFormat(center + glm::vec3(size / 2, size / 2, 0), color),
        VertexFormat(center + glm::vec3(0, 0, 0), color),
        VertexFormat(center + glm::vec3(-size / 2, size / 2, 0), color)
    };

    Mesh* life = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 7,
        1, 6, 7,
        6, 3, 5,
        3, 4, 5,
        1, 3, 6,
        1, 2, 3
    };

    life->InitFromData(vertices, indices);
    return life;
}

