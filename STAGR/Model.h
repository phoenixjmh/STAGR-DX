#pragma once
//#include "stb_image/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <string>
#include "Mesh.h"
#include "TextureLoader.h"
//#include "ShapeFactory.h"

class Model {
public:
    Model(const char* path/*, VAFlags flags*/);
    ~Model();
    ID3D11ShaderResourceView* loadEmbeddedTexture(const aiTexture* embeddedTexture);
    glm::vec3 Position;
    glm::vec3 LightPosition;
    float Size;

    void Draw(Shader& shader, unsigned int depthMap);

private:
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    ComPtr<ID3D11ShaderResourceView>& TextureFromFile(const char* path, const std::string& directory,const aiScene* scene ,bool gamma = false);
    const char* name;
    //VAFlags m_flags;
   std::vector<Texture> textures_loaded;
    /*std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string type_name);*/
    std::vector<Mesh> m_Meshes;
    std::string m_ModelDirectory;

    void loadModel(const char* path);
    void processNode(aiNode* node, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName,const aiScene* scene, const unsigned int texture_slot=0);
    void buildShape(); //used for non model-based loading
    void printDebugInfo();
};

 //Helper

