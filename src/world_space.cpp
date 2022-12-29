#include "world_space.h"

#include <iostream>

static float CalculateDensity(int x, int y, int z);

void WorldSpace::Generate()
{
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            for (int k = 0; k < DEPTH; k++)
            {
                Cube cube;

                cube.v0 = glm::vec3{ i, j, k };
                cube.v1 = glm::vec3{ i, j + 1, k };
                cube.v2 = glm::vec3{ i + 1, j + 1, k };
                cube.v3 = glm::vec3{ i + 1, j, k };
                cube.v4 = glm::vec3{ i, j, k + 1 };
                cube.v5 = glm::vec3{ i, j + 1, k + 1 };
                cube.v6 = glm::vec3{ i + 1, j + 1, k + 1 };
                cube.v7 = glm::vec3{ i + 1, j, k + 1 };
                
                m_cubes.push_back(cube);
            }
        }
    }
}

std::vector<Cube> WorldSpace::GetCubes()
{
    return m_cubes;
}

static float CalculateDensity(int x, int y, int z)
{
    return x + y + z;
}
