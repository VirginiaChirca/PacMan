#include "object2D-pacman.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include <string>
#include <math.h>

#define pi 3.1415926535897932384


Mesh* object2D::CreateCircle(const std::string& name, float numTriangles, float radiusX, float radiusY, glm::vec3 color)
{
    float arg;

    Mesh* circle = new Mesh(name);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    vertices.emplace_back(glm::vec3(0, 0, 0), color);

    for (int i = 0; i < numTriangles; i++)
    {
        arg = 2.0f * (float)M_PI * i / numTriangles;

        vertices.emplace_back(glm::vec3(cos(arg) * radiusX, sin(arg) * radiusY, 0), color);
        indices.push_back(i);
    }


    indices.push_back(numTriangles);
    indices.push_back(1);
    circle->InitFromData(vertices, indices);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    return circle;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float height,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangular = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangular->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangular->InitFromData(vertices, indices);
    return rectangular;
}

Mesh* object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}
//https://github.com/PatruDiana/EGC-tema1/blob/main/Tema1/Objects2D.cpp
Mesh* object2D::Heart(std::string name, glm::vec3 center1, float radius, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    float arg, x, y;
    glm::vec3 center = center1;
    Mesh* heart = new Mesh(name);
    vertices.emplace_back(center, color);
    for (int i = 0; i <= 50; i++) {
        arg = 2 * pi * i / 50;
        x = cos(arg) * radius;
        y = sin(arg) * radius;
        vertices.push_back(VertexFormat(center + glm::vec3(x, y, 0), color));
        indices.push_back(i);
    }
    indices.push_back(51);
    indices.push_back(0);
    glm::vec3 center2 = glm::vec3(center1.x + 2 * radius, center1.y, 0);
    vertices.emplace_back(center2, color);
    for (int i = 52; i <= 100; i++) {
        arg = 2 * pi * i / 48;
        x = cos(arg) * radius;
        y = sin(arg) * radius;
        vertices.push_back(VertexFormat(center2 + glm::vec3(x, y, 0), color));
        indices.push_back(i);
    }
    vertices.push_back(VertexFormat(center + glm::vec3(radius, -3 * radius, 0), color));
    indices.push_back(33);
    indices.push_back(102);
    indices.push_back(102);
    indices.push_back(93);
    heart->SetDrawMode(GL_TRIANGLE_FAN);
    heart->InitFromData(vertices, indices);
    return heart;

}


