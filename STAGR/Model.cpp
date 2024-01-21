#include "Model.h"
#include "Renderer.h"



Model::Model(const char* path/*, VAFlags flags*/)
//: m_flags(flags)
    :
    name(path)
{

    /* if (m_flags.shape == PrimitiveShape::NONE)
     {
         stbi_set_flip_vertically_on_load(true);*/
    loadModel(path);
    /* }
     else
     {
         buildShape();
     }*/
#ifdef DEBUG_IMPORT
    printDebugInfo();

#endif
}
void Model::printDebugInfo()
{
    for (auto& mesh : m_Meshes)
    {
        Log("Model", name);
        mesh.printTextureInfo();
    }
}
void Model::Draw(Shader& shader, unsigned int depthMap)
{
    for (auto& mesh : m_Meshes)
    {
        /*mesh.UpdateLightPosition(LightPosition);*/
        mesh.UpdateTransformMatrix(Size,Position);
        mesh.Draw(shader, depthMap);
    }
}

void Model::loadModel(const char* path)
{
    std::string filepath = static_cast<std::string>(path);
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
        return;
    }
    m_ModelDirectory = filepath.substr(0, filepath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    Log("File verified at ", m_ModelDirectory);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 positions;

        positions.x = mesh->mVertices[i].x;
        positions.y = mesh->mVertices[i].y;
        positions.z = mesh->mVertices[i].z;

        vertex.Position = positions;
        glm::vec3 normals = {
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z,
        };
        vertex.Normal = normals;
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            
        }
        else
        {
            vertex.TexCoords = XMFLOAT2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",scene);
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    std::vector<Texture> specular_maps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular",scene,1);
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    //std::vector<Texture> normal_maps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

    //std::vector<Texture> height_maps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    //textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    // we need normal and height maps
    return Mesh( vertices, indices,Position, textures );
}

Model::~Model() = default;

//void Model::buildShape()
//{
//    if (m_flags.shape == CUBE)
//    {
//        std::vector<Vertex> cube_verts = ShapeFactory::create_cube_no_index();
//        std::vector<Texture> textures;
//
//        const Texture diffuse = { TextureFromFile("../../textures/wood.png", "../../textures/wood.png"), (std::string)"texture_diffuse", (std::string)"textures/wood.png" };
//        const Texture specular = { TextureFromFile("../../textures/wood-spec.png", "../../textures/wood-spec.png"), (std::string)"texture_specular", (std::string)"textures/wood-spec.png" };
//        textures.push_back(diffuse);
//        textures.push_back(specular);
//
//        // dummy indices
//        std::vector<unsigned int> indices;
//        Mesh cube(cube_verts, indices, textures);
//        m_Meshes.push_back(cube);
//    }
//}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
    std::string typeName, const aiScene* scene,const unsigned int texture_slot)
{
    //Get device and ctx
    auto dev = Renderer::Get().GetDevice();
    auto con = Renderer::Get().GetContext();

    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {

            HRESULT hr;
            Texture texture;

            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
            if (embeddedTexture != nullptr)
                texture.textureResourceView = loadEmbeddedTexture(embeddedTexture);
            else {
                std::string filename = std::string(str.C_Str());
                filename = m_ModelDirectory + '/' + filename;
                std::wstring filename_w = std::wstring(filename.begin(), filename.end());
                hr = CreateWICTextureFromFile(dev.Get(), con.Get(), filename_w.c_str(), nullptr, &texture.textureResourceView);
                if (FAILED(hr)) {
                    std::cout << "Texture  Couldn't  be loaded!\n";
                }
            }
            texture.slot = texture_slot;
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            this->textures_loaded.push_back(texture);
        }
    }
    if (texture_slot == 1)
        Log("Loading Specular map");
    else {
        Log("Loading Diffuse texture");
    }
    return textures;
}


ID3D11ShaderResourceView* Model::loadEmbeddedTexture(const aiTexture* embeddedTexture) {

    auto dev = Renderer::Get().GetDevice();
    auto con = Renderer::Get().GetContext();


    HRESULT hr;
    ID3D11ShaderResourceView* texture = nullptr;

    if (embeddedTexture->mHeight != 0) {
        // Load an uncompressed ARGB8888 embedded texture
        D3D11_TEXTURE2D_DESC desc;
        desc.Width = embeddedTexture->mWidth;
        desc.Height = embeddedTexture->mHeight;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresourceData;
        subresourceData.pSysMem = embeddedTexture->pcData;
        subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
        subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

        ID3D11Texture2D* texture2D = nullptr;
        hr = dev->CreateTexture2D(&desc, &subresourceData, &texture2D);
        if (FAILED(hr))
            Log("Create Texture2D failed!");

        hr = dev->CreateShaderResourceView(texture2D, nullptr, &texture);
        if (FAILED(hr))
            Log("Create SRV failed!");

        return texture;
    }

    // mHeight is 0, so try to load a compressed texture of mWidth bytes
    const size_t size = embeddedTexture->mWidth;

    hr = CreateWICTextureFromMemory(dev.Get(), con.Get(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
    if (FAILED(hr))
        Log("Create Texture from memory failed!");

    return texture;
}



