#include "iplanner/data/mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace iplanner
{
Mesh::Mesh() = default;

Mesh::Mesh(const std::string& filename)
{
  Load(filename);
}

Mesh::~Mesh() = default;


// Copy constructors
void Mesh::Copy(const Mesh& rhs)
{
  positions_ = rhs.positions_;
  normals_ = rhs.normals_;
  tex_coords_ = rhs.tex_coords_;
  texture_filename_ = rhs.texture_filename_;
}

Mesh::Mesh(const Mesh& rhs)
  : AsyncLoader(rhs)
{
  Copy(rhs);
}

Mesh& Mesh::operator = (const Mesh& rhs)
{
  AsyncLoader::Copy(rhs);
  Copy(rhs);
  return *this;
}

// Move constructors
void Mesh::Move(Mesh&& rhs) noexcept
{
  positions_ = std::move(rhs.positions_);
  normals_ = std::move(rhs.normals_);
  tex_coords_ = std::move(rhs.tex_coords_);
  texture_filename_ = std::move(rhs.texture_filename_);
}

Mesh::Mesh(Mesh&& rhs) noexcept
  : AsyncLoader(std::move(rhs))
{
  Move(std::move(rhs));
}

Mesh& Mesh::operator = (Mesh&& rhs) noexcept
{
  AsyncLoader::Move(std::move(rhs));
  Move(std::move(rhs));
  return *this;
}


bool Mesh::LoadFunc(const std::string& filename)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
  if (scene == NULL)
    return false;

  auto mesh = std::make_shared<Mesh>();

  aiNode* scene_node = scene->mRootNode;

  // Assume only one mesh is contained in the file
  aiMesh* ai_mesh = scene->mMeshes[0];

  for (int i = 0; i < ai_mesh->mNumVertices; i++)
  {
    positions_.push_back(ai_mesh->mVertices[i].x);
    positions_.push_back(ai_mesh->mVertices[i].y);
    positions_.push_back(ai_mesh->mVertices[i].z);
  }

  if (ai_mesh->mNormals != NULL)
  {
    for (int i = 0; i < ai_mesh->mNumVertices; i++)
    {
      normals_.push_back(ai_mesh->mNormals[i].x);
      normals_.push_back(ai_mesh->mNormals[i].y);
      normals_.push_back(ai_mesh->mNormals[i].z);
    }
  }

  if (ai_mesh->mTextureCoords[0] != NULL)
  {
    for (int i = 0; i < ai_mesh->mNumVertices; i++)
    {
      tex_coords_.push_back(ai_mesh->mTextureCoords[0][i].x);
      tex_coords_.push_back(ai_mesh->mTextureCoords[0][i].y);
    }
  }

  for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
  {
    aiFace face = ai_mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
      indices_.push_back(face.mIndices[j]);
  }

  aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];
  int num_diffuse_textures = material->GetTextureCount(aiTextureType_DIFFUSE);
  for (int i = 0; i < num_diffuse_textures; i++)
  {
    aiString str;
    material->GetTexture(aiTextureType_DIFFUSE, i, &str);
    std::string texture_filename = str.C_Str();

    // Make absolute path
    texture_filename = filename.substr(0, filename.find_last_of("\\/") + 1) + texture_filename;

    texture_filename_ = texture_filename;
  }

  return true;
}
}
