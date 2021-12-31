//
// Created by BlackFlage on 20.12.2021.
//

#ifndef MAIN_RESOURCESMANAGER_H
#define MAIN_RESOURCESMANAGER_H

#include <filesystem>

#include "MeshLoader.h"

namespace vis
{
    class ResourcesManager
    {
    public:
        ResourcesManager();
        ~ResourcesManager() = default;

        Mesh*                    get_mesh(std::uint16_t a_id);
        std::uint16_t            load_mesh(const std::string& a_name);
        std::string              get_mesh_name_from_id(int a_id);
        std::vector<const char*> get_available_meshes();
        int                      get_mesh_id(const std::string& a_name);
        void                     load_meshes_in_folders(const std::filesystem::path& a_path);

        static void                              init();
        static std::shared_ptr<ResourcesManager> get_instance();
    private:
        static std::shared_ptr<ResourcesManager> m_instance;
        std::unique_ptr<MeshLoader> m_mesh_loader;
    };
}

#endif //MAIN_RESOURCESMANAGER_H
