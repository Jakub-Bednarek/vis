//
// Created by BlackFlage on 13.11.2021.
//

#include "Model.h"

#include <utility>
#include <glm/gtc/matrix_transform.hpp>

namespace vis
{
    Model::Model(const std::vector<std::string>& a_model_paths, const std::string& a_texture_path)
    :   m_position(glm::vec3(0.0f, 0.0f, -20.0f))
    {
        load_meshes(a_model_paths);
        m_texture = new Texture(a_texture_path);
    }

    Model::Model(const std::vector<std::string>& a_file_paths, glm::vec3 a_position)
            :   m_position(a_position)
    {
        load_meshes(a_file_paths);
    }

    Model::~Model()
    {
        for(auto& m : m_meshes)
        {
            delete m;
        }

        delete m_texture;
    }

    void Model::bind_texture() const
    {
        m_texture->bind();
    }

    const std::vector<Mesh*>& Model::get_meshes() const
    {
        return m_meshes;
    }

    const glm::mat4& Model::get_transform() const
    {
        glm::mat4 m_transform = glm::mat4(1.0f);
        m_transform = glm::translate(m_transform, m_position);

        return m_transform;
    }

    void Model::load_meshes(const std::vector<std::string>& a_file_paths)
    {
        for(const auto& s : a_file_paths)
        {
            m_meshes.push_back(OBJLoader::load_from_models(s));
        }
    }

    void Model::unbind_texture() const
    {
        m_texture->unbind();
    }
}
