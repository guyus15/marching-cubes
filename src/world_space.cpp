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
                m_world_density[i][j][k] = CalculateDensity(i, j, k);
            }
        }
    }
}

static float CalculateDensity(int x, int y, int z)
{
    return x + y + z;
}
