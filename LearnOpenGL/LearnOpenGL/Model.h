#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <memory>

#include "Mesh.h"

class Mesh;
class Shader;

class Model
{
public:
    Model(const std::string& path, bool gamma = false);
    void Draw(std::shared_ptr<Shader> shader);

private :
    //model Data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    bool gammaCorrection;

    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string& directory, bool gamma = false);
};

#endif