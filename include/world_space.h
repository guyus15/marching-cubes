#ifndef WORLD_SPACE_
#define WORLD_SPACE_

#include "glm/vec3.hpp"

#include <array>
#include <vector>

const int WIDTH = 10, HEIGHT = 10, DEPTH = 10;

struct Cube
{
    glm::vec3 v0, v1, v2, v3, v4, v5, v6, v7;
};

class WorldSpace
{
public:
    WorldSpace() = default;
    ~WorldSpace() = default;

    void Generate();
    std::vector<Cube> GetCubes();

private:
    std::vector<Cube> m_cubes;
};

#endif // WORLD_SPACE_
