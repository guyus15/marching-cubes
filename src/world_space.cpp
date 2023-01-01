#include "world_space.h"
#include "lookup_tables.h"

#include <iostream>
#include <vector>

const float DENSITY_THRESHOLD = 1.0f;

static float CalculateDensity(int x, int y, int z);
static std::vector<glm::vec3> EdgesToVertices(glm::vec3 v0, int* edges);

static float TempDensityCalc(const int vertexno);

void WorldSpace::Generate()
{
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
            glm::vec3 current_vertex = cube.vertices[i];
            //float vertex_density = CalculateDensity(current_vertex.x, current_vertex.y, current_vertex.z);
            float vertex_density = TempDensityCalc(i);

            if (vertex_density > DENSITY_THRESHOLD)
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
           
            std::vector<glm::vec3> edges_to_vertices = EdgesToVertices(cube.vertices[0], current_edges);

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
    return x + y + z;
}

static std::vector<glm::vec3> EdgesToVertices(glm::vec3 v0, int* edges)
{
    // This method will create a set of vertices for each edge connection,
    // relative to the position of the root vertex of the cube, v0.
   
    std::vector<glm::vec3> vertices; 

    std::cout << "Localising polygon edges: (" << edges[0] << ", " << edges[1] << ", " << edges[2] << ")" << std::endl;

    // Iterate over each edge in the polygon.
    for (int i = 0; i < 3; i++)
    {
        const int current_edge = edges[i];

        glm::vec3 current_vertex{};

        switch (current_edge)
        {       
        case 0:
            current_vertex = v0 + glm::vec3{ 0.0f, 0.5f, -0.0f };
            break;
        case 1:
            current_vertex = v0 + glm::vec3{ 0.5f, 1.0f, -0.0f };
            break;
        case 2:
            current_vertex = v0 + glm::vec3{ 1.0f, 0.5f, -0.0f };
            break;
        case 3:
            current_vertex = v0 + glm::vec3{ 0.5f, 0.0f, -0.0f };
            break;
        case 4:
            current_vertex = v0 + glm::vec3{ 0.0f, 0.5f, -1.0f };
            break;
        case 5:
            current_vertex = v0 + glm::vec3{ 0.5f, 1.0f, -1.0f };
            break;
        case 6:
            current_vertex = v0 + glm::vec3{ 1.0f, 0.5f, -1.0f };
            break;
        case 7:
            current_vertex = v0 + glm::vec3{ 0.5f, 0.0f, -1.0f };
            break;
        case 8:
            current_vertex = v0 + glm::vec3{ 0.0f, 0.0f, -0.5f };
            break;
        case 9:
            current_vertex = v0 + glm::vec3{ 0.0f, 1.0f, -0.5f };
            break;
        case 10:
            current_vertex = v0 + glm::vec3{ 1.0f, 1.0f, -0.5f };
            break;
        case 11:
            current_vertex = v0 + glm::vec3{ 1.0f, 0.0f, -0.5f };
            break;
        default: break;
        }

        vertices.push_back(current_vertex);
    }

    return vertices;
}

static float TempDensityCalc(const int vertexno)
{
    unsigned char current_vertex = 1 << vertexno;
    unsigned char active_vertices = 0b10111000;

    if (current_vertex & active_vertices)
    {
        return 2.0f;
    }

    return 0.0f;
}
