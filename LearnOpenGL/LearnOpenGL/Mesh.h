#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>

class Shader;

#define MAX_BONE_INFLUENCE 4
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE]; //bone indexes which will influence this vertex
    float m_Weights[MAX_BONE_INFLUENCE]; //weights from each bone
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(std::shared_ptr<Shader> shader);

private :
    //mesh data
    std::vector<Vertex>         vertices;
    std::vector<unsigned int>   indices;
    std::vector<Texture>        textures;
    
    //render data
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void SetupMesh();
};

#endif