#include "Model.h"
#include "..\inc\Model.h"

Model::Model(const std::string& path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate
        | aiProcess_FlipUVs
        | aiProcess_PreTransformVertices
        | aiProcess_GenSmoothNormals
        | aiProcess_JoinIdenticalVertices);
    import.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    std::string directory = path.substr(0, path.find_last_of('/'));
   

    aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[0]];
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // positions
        vertex.m_Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        // normals
        if (mesh->HasNormals())
            vertex.m_Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        // texture coordinates
        if (mesh->mTextureCoords[0])
            vertex.m_TextureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.m_TextureCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    // go through all faces of the node
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve indicies of the face
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // get material
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    Material mat;
    aiColor4D ambient;
    aiColor4D diffuse;
    aiColor4D specular;
    float shininess;

    aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
    aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
    aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
    aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

    mat.m_Ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
    mat.m_Diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
    mat.m_Specular = glm::vec3(specular.r, specular.g, specular.b);
    mat.m_Shine = shininess;

    Texture texture;

    // get diffuse texture
    if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        std::string fullPath = directory + "/" + str.C_Str();
        Texture texture(fullPath);
        m_Texture = texture;
    }

    m_Geometry = Geometry(vertices, indices);
    m_Position = glm::vec3(0.0f);
}

Model::Model(const Geometry& geometry, const Material& material)
{
    m_Geometry = geometry;
    m_Material = material;
}

void Model::Draw(const Shader& shader)
{
    shader.SetMat4("model", m_Geometry.GetModelMatrix());
    shader.SetBool("diffuseUsed", m_Texture.Initialized());
    m_Texture.Bind(GL_TEXTURE7);




    m_Geometry.Draw();
    shader.SetBool("diffuseUsed", false);
    glActiveTexture(GL_TEXTURE0);
}

void Model::DrawNoColor(const Shader& shader)
{
    shader.SetMat4("model", m_Geometry.GetModelMatrix());
    m_Geometry.Draw();
}

void Model::SetScale(const glm::vec3& scale)
{
    m_Geometry.SetScale(scale);
}

void Model::SetPosition(const glm::vec3& pos)
{
    m_Position = pos;
    m_Geometry.SetPosition(pos);
}

glm::vec3 Model::GetPosition() const
{
    return m_Position;
}
