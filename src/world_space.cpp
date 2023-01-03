#include "world_space.h"
#include "lookup_tables.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

const float DENSITY_THRESHOLD = 0.0f;

static float CalculateDensity(int x, int y, int z);
static std::vector<glm::vec3> EdgesToVertices(const Cube& cube, int* edges);
static glm::vec3 InterpolateVertex(glm::vec3 p1, glm::vec3 p2, float val1, float val2);

static float TempDensityCalc(const int vertexno);

void WorldSpace::Generate()
{
    // Seed random number
    srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            for (int k = 0; k < DEPTH; k++)
            {
                Cube cube;

                cube.vertices[0] = glm::vec3{ i, j, k };
                cube.vertices[1] = glm::vec3{ i, j + 1, k };
                cube.vertices[2] = glm::vec3{ i + 1, j + 1, k };
                cube.vertices[3] = glm::vec3{ i + 1, j, k };
                cube.vertices[4] = glm::vec3{ i, j, k - 1 };
                cube.vertices[5] = glm::vec3{ i, j + 1, k - 1 };
                cube.vertices[6] = glm::vec3{ i + 1, j + 1, k - 1 };
                cube.vertices[7] = glm::vec3{ i + 1, j, k - 1 };
               
                
                cube.densities[0] = CalculateDensity(i, j, k);
                cube.densities[1] = CalculateDensity(i, j + 1, k);
                cube.densities[2] = CalculateDensity(i + 1, j + 1, k);
                cube.densities[3] = CalculateDensity(i + 1, j, k);
                cube.densities[4] = CalculateDensity(i, j, k - 1);
                cube.densities[5] = CalculateDensity(i, j + 1, k - 1);
                cube.densities[6] = CalculateDensity(i + 1, j + 1, k - 1);
                cube.densities[7] = CalculateDensity(i + 1, j, k - 1);
                
                /*
                cube.densities[0] = TempDensityCalc(0);
                cube.densities[1] = TempDensityCalc(1);
                cube.densities[2] = TempDensityCalc(2);
                cube.densities[3] = TempDensityCalc(3);
                cube.densities[4] = TempDensityCalc(4);
                cube.densities[5] = TempDensityCalc(5);
                cube.densities[6] = TempDensityCalc(6);
                cube.densities[7] = TempDensityCalc(7);
                */
                m_cubes.push_back(cube);
            }
        }
    }
}

std::vector<float> WorldSpace::CreateTriangles()
{
    std::vector<float> total_vertices;

    for (const auto& cube : m_cubes)
    {
        unsigned char case_index = 0x00;

        for (int i = 0; i < 8; i++)
        {
            float current_density = cube.densities[i];

            if (current_density >= DENSITY_THRESHOLD)
            {
                case_index |= 1 << i;
            }
        }

        if (case_index == 0 || case_index == 255)
        {
            // Cube won't contain any triangles, so skip.
            continue;
        }

        int num_polygons = num_polys[case_index];

        for (int i = 0; i < num_polygons; i++)
        {
            // Iterate over each vertex in the edge connect list and localise them
            // so they're relative to the current cube (eventually this will be interpolated).

           int* current_edges = edge_connect_list[case_index][i];


           // TODO: This method, CreateTriangles(), should return a vector containing all the vertices
           // which need to be sent to a vertex buffer to produce the triangles to be rendered.
           
            std::vector<glm::vec3> edges_to_vertices = EdgesToVertices(cube, current_edges);

            std::cout << "Localised vertices for polygon - " << i << ":" << std::endl;

            for (const auto& localised_edge : edges_to_vertices)
            {
                std::cout << "Adding vertex: (" << localised_edge.x << ", " << localised_edge.y << ", " << localised_edge.z << ")" << std::endl;

                total_vertices.push_back(localised_edge.x);
                total_vertices.push_back(localised_edge.y);
                total_vertices.push_back(localised_edge.z);
            }
        }
    }

    return total_vertices;
}

std::vector<Cube> WorldSpace::GetCubes()
{
    return m_cubes;
}

static float CalculateDensity(int x, int y, int z)
{
    float density = -y;

    return density + (x/2) + (z/2);
}

static std::vector<glm::vec3> EdgesToVertices(const Cube& cube, int* edges)
{
    // This method will create a set of vertices for each edge connection,
    // relative to the position of the root vertex of the cube, v0.
   
    std::vector<glm::vec3> localised_vertices; 

    std::cout << "Localising polygon edges: (" << edges[0] << ", " << edges[1] << ", " << edges[2] << ")" << std::endl;

    // Iterate over each edge in the polygon.
    for (int i = 0; i < 3; i++)
    {
        const int current_edge = edges[i];

        glm::vec3 current_vertex{};

        switch (current_edge)
        {       
        case 0:
            current_vertex = InterpolateVertex(cube.vertices[0], cube.vertices[1], cube.densities[0], cube.densities[1]);
            break;
        case 1:
            current_vertex = InterpolateVertex(cube.vertices[1], cube.vertices[2], cube.densities[1], cube.densities[2]);
            break;
        case 2:
            current_vertex = InterpolateVertex(cube.vertices[2], cube.vertices[3], cube.densities[2], cube.densities[3]);
            break;
        case 3:
            current_vertex = InterpolateVertex(cube.vertices[3], cube.vertices[0], cube.densities[3], cube.densities[0]);
            break;
        case 4:
            current_vertex = InterpolateVertex(cube.vertices[4], cube.vertices[5], cube.densities[4], cube.densities[5]);
            break;
        case 5:
            current_vertex = InterpolateVertex(cube.vertices[5], cube.vertices[6], cube.densities[5], cube.densities[6]);
            break;
        case 6:
            current_vertex = InterpolateVertex(cube.vertices[6], cube.vertices[7], cube.densities[6], cube.densities[7]);
            break;
        case 7:
            current_vertex = InterpolateVertex(cube.vertices[7], cube.vertices[4], cube.densities[7], cube.densities[4]);
            break;
        case 8:
            current_vertex = InterpolateVertex(cube.vertices[0], cube.vertices[4], cube.densities[0], cube.densities[4]);
            break;
        case 9:
            current_vertex = InterpolateVertex(cube.vertices[1], cube.vertices[5], cube.densities[1], cube.densities[5]);
            break;
        case 10:
            current_vertex = InterpolateVertex(cube.vertices[2], cube.vertices[6], cube.densities[2], cube.densities[6]);
            break;
        case 11:
            current_vertex = InterpolateVertex(cube.vertices[3], cube.vertices[7], cube.densities[3], cube.densities[7]);
            break;
        default: break;
        }

        localised_vertices.push_back(current_vertex);
    }

    return localised_vertices;
}

static glm::vec3 InterpolateVertex(glm::vec3 p1, glm::vec3 p2, float val1, float val2)
{
    if (abs(DENSITY_THRESHOLD - val1) < 0.00001)
        return p1;
    if (abs(DENSITY_THRESHOLD - val2) < 0.00001)
        return p2;
    if (abs(val1 - val2) < 0.00001)
        return p1;

    std::cout << "Made it here!" << std::endl;

    float mu = (DENSITY_THRESHOLD - val1) / (val2 - val1);

    glm::vec3 p;
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return p;
}

static float TempDensityCalc(const int vertexno)
{
    unsigned char current_vertex = 1 << vertexno;
    unsigned char active_vertices = 0b00000001;

    if (current_vertex & active_vertices)
    {
        return 1.0f;
    }

    return -1.0f;
}
