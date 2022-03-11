#include "Object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Object2D::CreateRectangle(const std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-width / 2, -height / 2, 0), color),
        VertexFormat(corner + glm::vec3(width / 2, -height / 2, 0), color),
        VertexFormat(corner + glm::vec3(width / 2, height / 2, 0), color),
        VertexFormat(corner + glm::vec3(-width / 2, height / 2, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
    rectangle->SetDrawMode(GL_TRIANGLES);
    rectangle->InitFromData(vertices, indices);

    return rectangle;
}

Mesh* Object2D::CreateCircle(std::string name, float nr_tring, float radius, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    float arg;
    
    vertices.emplace_back(color, color);
    for (int i = 0; i < nr_tring; i++)
    {
        arg = 2 * 3.2f * i / nr_tring;

        vertices.emplace_back(glm::vec3(cos(arg) * radius, sin(arg) * radius, 0), color);
        indices.push_back(i);
    }
    indices.push_back(nr_tring);

    Mesh* circle = new Mesh(name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);

    circle->InitFromData(vertices, indices);
    return circle;
}

