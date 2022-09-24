#include "Model.h"

#include <iostream>

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"


Model::Model(const std::string& path, bool gamma)
    :gammaCorrection(gamma)
{
    LoadModel(path);
}

void Model::Draw(std::shared_ptr<Shader> shader)
{
    for(unsigned int i = 0; i < meshes.size(); ++i)
    {
        meshes[i]->Draw(shader);       
    }
}

void Model::Clear()
{
    for(int i =0; i < meshes.size(); ++i)
    {
        meshes[i]->Clear();
    }

    for(int i = 0; i < textures_loaded.size(); ++i)
    {
        glDeleteTextures(1, &textures_loaded[i].id); 
    }
}

void Model::LoadModel(const std::string& path)
{
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    
    Assimp::Importer importer;
    
    //post-processing options
    //aiProcess_Triangulate : if the model does not (entirely) consist of triangles, it should transform all the model's primitive shapes to triangles first.
    //aiProcess_FlipUVs : flips the texture coordinates on the y-axis where necessary during processing
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr)
    {
        std::cout << "Model.LoadModel Failed " + path << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        // process vertex positions, normals and texture coordinates
        Vertex vertex;

        glm::vec3 vector;

        //position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        //normals
        if(mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;    
        }
        
        //texture coordinate
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            glm::vec2 vec;
            
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;

            //tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;

            //bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        
        vertices.push_back(vertex);
    }

    // process indices
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        //diffuse maps
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        //specular maps
        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        //normal maps
        std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        //height maps
        std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        //check duplication of texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); ++j)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        //Load texture
        if(skip == false)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    int width = 0;
    int height = 0;
    int nrChannels = 0;
    unsigned int textureID = 0;
    stbi_set_flip_vertically_on_load(true); //OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 at the top of the y-axis
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    //Generate texture
    if (data != nullptr)
    {
        GLenum sourceImageFormat;
        if(nrChannels == 1)
        {
            sourceImageFormat = GL_RED;
        }
        else if(nrChannels == 3)
        {
            sourceImageFormat = GL_RGB;
        }
        else if(nrChannels == 4)
        {
            sourceImageFormat = GL_RGBA;
        }
		
        //Create and bind Texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        //Give texture data to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, sourceImageFormat, width, height, 0, sourceImageFormat, GL_UNSIGNED_BYTE, data); //Target for generating texture, mipmap level, format to store, width, height, legacy, source image's format, source image's data type
        glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap

        // set the texture wrapping/filtering options (on the currently bound texture object)
        //Texture Wrapping, s, t (and r if you're using 3D textures) equivalent to x,y,z
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //Border color
        /*float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);*/

        //Texture Filtering
        //A common mistake is to set one of the mipmap filtering options as the magnification filter. This doesn't have any effect since mipmaps are primarily used for when textures get downscaled
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
        stbi_image_free(data); //After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory
    }
    else
    {
        std::cout << "Failed to load texture " << filename << std::endl;
        stbi_image_free(data); //After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}