#ifndef WORLD_SPACE_
#define WORLD_SPACE_

#include "glm/vec3.hpp"

#include <array>
#include <vector>

const int WIDTH = 1, HEIGHT = 1, DEPTH = 1;

struct Cube
{
    glm::vec3 vertices[8];
};

class WorldSpace
{
public:
    WorldSpace() = default;
    ~WorldSpace() = default;

    void Generate();

    std::vector<float> CreateTriangles();
    std::vector<Cube> GetCubes();

private:
    std::vector<Cube> m_cubes;
};

#endif // WORLD_SPACE_
