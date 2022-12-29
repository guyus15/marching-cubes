#ifndef WORLD_SPACE_
#define WORLD_SPACE_

#include <array>

const int WIDTH = 100, HEIGHT = 100, DEPTH = 100;

typedef std::array<std::array<std::array<float, DEPTH>, HEIGHT>, WIDTH> WorldDensity;

class WorldSpace
{
public:
    WorldSpace() = default;
    ~WorldSpace() = default;

    void Generate();

private:
    WorldDensity m_world_density;
};

#endif // WORLD_SPACE_
