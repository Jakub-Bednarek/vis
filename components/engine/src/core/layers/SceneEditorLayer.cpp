//
// Created by BlackFlage on 23.12.2021.
//

#include <algorithm>

#include <Windows.h>

#include "SceneEditorLayer.h"
#include "Application.h"
#include "nfd.h"
#include "ColorSchemeEditor.h"
#include "managers/MeshManager.h"
#include "managers/ResourcesManager.h"

#define DEFAULT_RIGHT_PADDING 10
#define DEFAULT_LEFT_PADDING 2 * DEFAULT_RIGHT_PADDING
#define TEXT_LEFT(LABEL, FUNC) ImGui::Text(LABEL); ImGui::SameLine(); FUNC

namespace vis
{
    std::shared_ptr<MainManager> MainManager::m_instance;

    void SceneEditorLayer::on_attach()
    {
        initialize_ecs_system();
        initialize_scene_hierarchy();
        initialize_assets_panel();

        std::vector<std::string> skybox_textures = { R"(..\engine_assets\skyboxes\default\right.jpg)",
                                                     R"(..\engine_assets\skyboxes\default\left.jpg)",
                                                     R"(..\engine_assets\skyboxes\default\top.jpg)",
                                                     R"(..\engine_assets\skyboxes\default\bottom.jpg)",
                                                     R"(..\engine_assets\skyboxes\default\front.jpg)",
                                                     R"(..\engine_assets\skyboxes\default\back.jpg)"};

        m_skybox            = new Skybox(skybox_textures);
        m_editor_camera     = new Camera(glm::vec3(0.0f, 2.0f, 10.0f));
        m_editor_shader     = Shader::create_shader(R"(..\engine_assets\shaders\vertex.glsl)",R"(..\engine_assets\shaders\fragmentNoTex.glsl)");
        m_grid_shader       = Shader::create_shader(R"(..\engine_assets\shaders\vertex_grid.glsl)", R"(..\engine_assets\shaders\fragment_grid.glsl)");
        m_skybox_shader     = Shader::create_shader(R"(..\engine_assets\shaders\vertex_skybox.glsl)", R"(..\engine_assets\shaders\fragment_skybox.glsl)");
        m_default_mesh_path = R"(..\engine_assets\meshes\)";
        m_mouse_move_mode_enabled = false;

        CheckGLError();

        Renderer::set_camera(m_editor_camera);
        Renderer::set_shader(m_editor_shader);

        m_grid.create(200, 200);
    }

    void SceneEditorLayer::on_detach()
    {
        for (auto it = m_icons.begin(); it != m_icons.end(); it++) {
            delete it->second;
        }

        delete m_editor_camera;
        delete m_grid_shader;
        delete m_editor_shader;
        delete m_skybox_shader;
    }

    void SceneEditorLayer::on_event(vis::Event &a_event)
    {
        EventDispatcher dispatcher(a_event);
        dispatcher.dispatch<WindowResizeEvent>([this](auto &&event) { on_window_resize_event(std::forward<decltype(event)>(event)); });
    }

    void SceneEditorLayer::on_update(float a_dt)
    {
        m_physics_system->on_update(a_dt);

        if(m_mouse_move_mode_enabled)
        {
            move_camera(a_dt);
            m_editor_camera->recalculate_direction_vector(INPUT->get_mouse_delta_x(), INPUT->get_mouse_delta_y());
        }
    }

    void SceneEditorLayer::on_render()
    {
        Renderer::render_skybox(m_skybox, m_skybox_shader);
        Renderer::render_grid(m_grid, m_grid_shader);
        m_renderer_system->on_render();
    }

    void SceneEditorLayer::on_imgui_render()
    {
        render_imgui_ecs_window();
        render_scene_hierarchy();
        render_assets_panel();

        bool open = true;
        ImGui::Begin("Scene viewport", &open, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);

        render_menu_bar();

        ImVec2 dimensions = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        ImGui::Image((void *) Application::get_instance()->get_framebuffer_texture_id(), dimensions);

        if((m_mouse_move_mode_enabled || ImGui::IsWindowHovered()) && INPUT->is_mouse_button_clicked(1))
        {
            m_mouse_move_mode_enabled = true;
        }
        else
        {
            m_mouse_move_mode_enabled = false;
        }

        ImGui::End();
    }

    void SceneEditorLayer::initialize_ecs_system()
    {
        MainManager::init();

        //Register all available components
        MainManager::get_instance()->register_component<Transform>();
        MainManager::get_instance()->register_component<Color>();
        MainManager::get_instance()->register_component<RigidBody>();
        MainManager::get_instance()->register_component<MeshComponent>();
        MainManager::get_instance()->register_component<SceneCamera>();

        //Register all system
        m_physics_system = MainManager::get_instance()->register_system<PhysicsSystem>();
        m_renderer_system = MainManager::get_instance()->register_system<RendererSystem>();

        //Set system signatures
        Signature phys_signature;
        phys_signature.set(MainManager::get_instance()->get_component_type<Transform>(), true);
        phys_signature.set(MainManager::get_instance()->get_component_type<RigidBody>(), true);
        MainManager::get_instance()->set_system_signature<PhysicsSystem>(phys_signature);

        Signature rend_signature;
        rend_signature.set(MainManager::get_instance()->get_component_type<MeshComponent>(), true);
        rend_signature.set(MainManager::get_instance()->get_component_type<Color>(), true);
        rend_signature.set(MainManager::get_instance()->get_component_type<Transform>(), true);
        MainManager::get_instance()->set_system_signature<RendererSystem>(rend_signature);

        m_components_names = {"Transform", "Color", "Mesh", "RigidBody", "Camera"};
    }

    void SceneEditorLayer::render_transform_component(std::uint16_t a_id)
    {
        Signature sig = MainManager::get_instance()->get_entity_signature(a_id);

        if (sig[MainManager::get_instance()->get_component_type<Transform>()])
        {
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Transform")) {
                ImGui::Separator();
                auto &transform = MainManager::get_instance()->get_component<Transform>(a_id);

                float pos_to_add = ImGui::CalcTextSize("Position").x;
                render_transform_slider("Position", &transform.m_position, pos_to_add, 0.1f);
                render_transform_slider("Rotation", &transform.m_rotation, pos_to_add, 0.1f);
                render_transform_slider("Scale", &transform.m_scale, pos_to_add, 0.05f, 0.0f);
            }
        }
    }

    void SceneEditorLayer::render_color_component(std::uint16_t a_id)
    {
        Signature sig = MainManager::get_instance()->get_entity_signature(a_id);

        if (sig[MainManager::get_instance()->get_component_type<Color>()])
        {
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Color")) {
                ImGui::Separator();
                auto &color = MainManager::get_instance()->get_component<Color>(a_id);

                ImGui::ColorEdit3("Mesh color", &color.m_color[0]);
            }
        }
    }

    void SceneEditorLayer::render_mesh_component(std::uint16_t a_id)
    {
        Signature sig = MainManager::get_instance()->get_entity_signature(a_id);

        if (sig[MainManager::get_instance()->get_component_type<MeshComponent>()])
        {
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Mesh")) {
                ImGui::Separator();
                std::vector<const char *> available_meshes = MeshManager::get()->get_all_meshes_names();

                int &mesh_id = MainManager::get_instance()->get_component<MeshComponent>(a_id).m_id;
                int selection = 0;
                std::string name = MeshManager::get()->get_name(mesh_id);

                for (int i = 0; i < available_meshes.size(); i++) {
                    if (available_meshes.at(i) == name) {
                        selection = i;
                        break;
                    }
                }
                ImGui::Combo("Meshes combo", &selection, &(available_meshes[0]), available_meshes.size());

                mesh_id = MeshManager::get()->get_id(available_meshes.at(selection));
                auto mesh = MeshManager::get()->get_mesh(mesh_id);
                ImGui::Text("Mesh id: %d", mesh_id);
                ImGui::Text("Vertices: %d", (int) mesh->get_vertices().size());
                ImGui::Text("Indices: %d", (int) mesh->get_indices().size());
            }
        }
    }

    void SceneEditorLayer::render_camera_component(std::uint16_t a_id)
    {
        Signature sig = MainManager::get_instance()->get_entity_signature(a_id);

        if(sig[MainManager::get_instance()->get_component_type<SceneCamera>()])
        {
            ImGui::Separator();

            if(ImGui::CollapsingHeader("Camera"))
            {
                ImGui::Separator();
                auto& camera = MainManager::get_instance()->get_component<SceneCamera>(a_id);

                ImGui::Text("Use skybox"); ImGui::SameLine();
                ImGui::Checkbox("##Use skybox", &camera.get_use_skybox());

                ImGui::Text("Clear color"); ImGui::SameLine();
                ImGui::ColorEdit3("##Clear color", &(camera.get_clear_color()[0]));

                TEXT_LEFT("zNear", ImGui::InputFloat("##zNear", &camera.get_z_near()));
                TEXT_LEFT("zFar", ImGui::InputFloat("##zFar", &camera.get_z_far()));
                TEXT_LEFT("FOV", ImGui::InputFloat("##FOV", &camera.get_fov()));
            }
        }
    }

    void SceneEditorLayer::render_add_component_button(std::uint16_t a_id)
    {
        ImVec2 button_size = ImVec2(ImGui::GetWindowWidth() * 0.8f, ImGui::GetTextLineHeight() * 1.4f);

        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - button_size.x) / 2.0f);
        if (ImGui::Button("Add Component", button_size))
            ImGui::OpenPopup("Components list");

        if (ImGui::BeginPopup("Components list")) {
            for (auto &m_components_name: m_components_names) {
                if (ImGui::MenuItem(m_components_name)) {
                    add_component_to_entity(a_id, m_components_name);
                }
            }

            ImGui::EndPopup();
        }
    }

    void SceneEditorLayer::add_component_to_entity(std::uint16_t a_id, const char *a_component_name)
    {
        if (std::strcmp(a_component_name, "Transform") == 0)
        {
            MainManager::get_instance()->add_component(a_id, Transform{
                    .m_position = glm::vec3(0.0f),
                    .m_rotation = glm::vec3(0.0f),
                    .m_scale = glm::vec3(1.0f)
            });
        }
        else if (std::strcmp(a_component_name, "Color") == 0)
        {
            MainManager::get_instance()->add_component(a_id, Color{.m_color = glm::vec3(0.7f)});
        }
        else if (std::strcmp(a_component_name, "Mesh") == 0)
        {
            MeshComponent mesh_comp = {.m_id =  MeshManager::get()->load_mesh(m_default_mesh_path + "cube.obj")};

            if (mesh_comp.m_id != MAX_COMPONENTS) {
                MainManager::get_instance()->add_component(a_id, mesh_comp);
            }
        }
        else if (std::strcmp(a_component_name, "RigidBody") == 0)
        {
            MainManager::get_instance()->add_component(a_id, RigidBody{.vel_x = 0.0f, .vel_y = 0.0f, .vel_z = 0.0f});
        }
        else if(std::strcmp(a_component_name, "Camera") == 0)
        {
            MainManager::get_instance()->add_component(a_id, SceneCamera());
        }
    }

    void SceneEditorLayer::on_window_resize_event(WindowResizeEvent &a_event)
    {
        m_editor_camera->recalculate_perspective(a_event.get_width(), a_event.get_height());
    }

    void SceneEditorLayer::render_imgui_ecs_window()
    {
        std::uint16_t id = MainManager::get_instance()->get_current_entity();

        ImGui::Begin("Properties");

        if (id != MAX_ENTITIES) {
            Theme current_theme = ColorSchemeEditor::get_current_theme();

            ImGui::PushStyleColor(ImGuiCol_Header, ColorSchemeEditor::get_color(current_theme.background, ColorSchemeEditor::m_alpha_100));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ColorSchemeEditor::get_color(current_theme.background, ColorSchemeEditor::m_alpha_100));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ColorSchemeEditor::get_color(current_theme.background, ColorSchemeEditor::m_alpha_100));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

            render_transform_component(id);
            render_color_component(id);
            render_mesh_component(id);
            render_camera_component(id);

            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor(3);

            ImGui::Separator();
            render_add_component_button(id);
        }

        ImGui::End();

        //Gonna be deleted soon
        bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    void SceneEditorLayer::initialize_scene_hierarchy()
    {
        m_scene_hierarchy_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        m_selected_entity = MAX_ENTITIES;
        m_id_to_perform_action = MAX_ENTITIES;
    }

    void SceneEditorLayer::render_scene_hierarchy()
    {
        ImGui::Begin("Scene Hierarchy");
        bool selected = false;
        ImGuiTreeNodeFlags flags = m_scene_hierarchy_flags;
        std::uint16_t i = 0;

        if (ImGui::TreeNode(SceneManager::get()->get_current_scene()->get_name().c_str())) {
            auto &m_scene_entities = SceneManager::get()->get_current_scene()->get_entities();
            for (auto it = m_scene_entities.begin(); it != m_scene_entities.end(); it++, i++) {
                if (m_selected_entity == i) {
                    selected = true;
                    MainManager::get_instance()->set_current_entity(*it);
                } else
                    selected = false;

                if (ImGui::Selectable(MainManager::get_instance()->get_entity(*it).get_name().c_str(), selected)) {
                    m_selected_entity = i;
                    m_id_to_perform_action = MAX_ENTITIES;
                }
                if (!m_mouse_move_mode_enabled && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
                    m_id_to_perform_action = *it;
                }
            }

            ImGui::TreePop();
        }

        render_hierarchy_popup();

        ImGui::End();
    }

    void SceneEditorLayer::render_hierarchy_popup()
    {
        if (!m_mouse_move_mode_enabled && ImGui::IsWindowHovered(ImGuiFocusedFlags_RootWindow)
            && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            ImGui::OpenPopup("Options");

        if (ImGui::BeginPopup("Options")) {
            if (ImGui::MenuItem("Copy")) {

            }
            if (ImGui::MenuItem("Paste")) {

            }
            if (ImGui::MenuItem("Rename")) {

            }
            if (ImGui::MenuItem("Delete")) {
                delete_entity(m_id_to_perform_action);
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("3D")) {
                if (ImGui::MenuItem("Empty")) {
                    SceneManager::get()->get_current_scene()->add_entity(EntityType::EMPTY);
                }
                if (ImGui::MenuItem("Cube")) {
                    SceneManager::get()->get_current_scene()->add_entity(EntityType::CUBE);
                }
                if (ImGui::MenuItem("Sphere")) {
                    SceneManager::get()->get_current_scene()->add_entity(EntityType::SPHERE);
                }

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }
    }

    void SceneEditorLayer::rename_entity()
    {
        LOG_INFO("RENAME ENTITY");
    }

    void SceneEditorLayer::copy_entity()
    {
        LOG_INFO("COPY ENTITY");
    }

    void SceneEditorLayer::paste_entity()
    {
        LOG_INFO("PASTE ENTITY");
    }

    void SceneEditorLayer::delete_entity(std::uint16_t a_id)
    {
        if (a_id != MAX_ENTITIES) {
            SceneManager::get()->get_current_scene()->remove_entity(a_id);
            MainManager::get_instance()->destroy_entity(a_id);
            MainManager::get_instance()->set_current_entity(MAX_ENTITIES);
            m_id_to_perform_action = MAX_ENTITIES;
            m_selected_entity = MAX_ENTITIES;
        }
    }

    void SceneEditorLayer::initialize_assets_panel()
    {
        m_max_items_in_row = 6;
        m_selected_path = "";
        m_default_icon_size = ImVec2(64, 64);
        m_mini_icon_zoom = 0.25f;
        m_spacing = 10.0f;
        m_renamed_path = "";
        m_selected_path = "..\\res\\assets";
        m_assets_path = std::filesystem::path("..\\res\\assets");

        std::filesystem::path default_assets(DEFAULT_ASSETS_PATH);

        if (!std::filesystem::exists(default_assets) && !std::filesystem::is_directory(default_assets)) {
            std::filesystem::create_directories(default_assets);
        }

        load_icons();

        ResourcesManager::get()->load_meshes_in_folders(m_assets_path);
    }

    void SceneEditorLayer::render_assets_panel()
    {
        bool show = true;
        if (ImGui::Begin("Assets browser", &show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                                                  ImGuiWindowFlags_NoScrollWithMouse)) {
            if (ImGui::BeginTabBar("Assets browser tabs", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Browser")) {
                    //Filesystem child
                    ImGui::BeginTable("Filesystem", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV |
                                                       ImGuiTableFlags_RowBg);

                    ImGui::TableNextRow(0, ImGui::GetWindowHeight() * 0.05f);
                    ImGui::TableNextColumn();

                    render_import_button();

                    ImGui::TableNextColumn();
                    render_directories_buttons();

                    ImGui::TableNextRow(0, ImGui::GetWindowHeight() * 0.85f);

                    ImGui::Separator();

                    std::filesystem::path root_dir(DEFAULT_ASSETS_PATH);

                    render_dir_tree(root_dir.parent_path());

                    //Assets view child
                    ImGui::TableNextColumn();
                    std::filesystem::path assets_view_path(m_selected_path);

                    ImGui::SetWindowFontScale(0.8f);
                    render_files_view_table(m_selected_path);
                    ImGui::SetWindowFontScale(1.0f);

                    ImGui::EndTable();

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Console")) {
                    if (ImGui::Button("Clear"))
                    {
                        SceneConsole::get_instance()->clear();
                    }

                    ImGui::SameLine();
                    if(ImGui::Button("Add xd"))
                    {
                        SceneConsole::get_instance()->log_info("XDDDDD");
                    }

                    ImGui::Separator();

                    SceneConsole::get_instance()->draw();

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }

        ImGui::End();
    }

    void SceneEditorLayer::load_icons()
    {
        m_icons.insert({"closed_dir", new Texture(R"(..\engine_assets\textures\closed_folder_icon.png)")});
        m_icons.insert({"opened_dir", new Texture(R"(..\engine_assets\textures\opened_folder_icon.png)")});
        m_icons.insert({"full_dir", new Texture(R"(..\engine_assets\textures\full_folder_icon.png)")});
        m_icons.insert({"right_arrow", new Texture(R"(..\engine_assets\textures\right_arrow.png)")});
        m_icons.insert({"default", new Texture(R"(..\engine_assets\textures\file_icon.png)")});
    }

    void SceneEditorLayer::render_dir_tree(const std::filesystem::path &a_path)
    {
        ImVec2 icon_dimensions = ImVec2(m_default_icon_size.x * m_mini_icon_zoom,
                                        m_default_icon_size.y * m_mini_icon_zoom);

        ImGui::TableNextColumn();

        ImGui::BeginChild("File browser child", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (std::filesystem::exists(a_path)) {
            for (const auto &entry: std::filesystem::directory_iterator(a_path)) {
                render_entry(entry, icon_dimensions);
            }
        }

        ImGui::EndChild();
    }

    void SceneEditorLayer::render_files_view_table(const std::filesystem::path &a_path)
    {
        ImGui::BeginChild("Files view table");

        if (std::filesystem::is_directory(a_path)) {
            if (std::filesystem::is_empty(a_path)) {
                float pos_x = ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() / 2.0f) -
                              (ImGui::CalcTextSize("Folder is empty!").x / 2.0f);
                float pos_y = ImGui::GetCursorPosY() + (ImGui::GetWindowHeight() / 2.0f) -
                              (ImGui::GetTextLineHeight() / 2.0f);

                ImGui::SetCursorPos({pos_x, pos_y});

                ImGui::SetWindowFontScale(1.5f);
                ImGui::Text("Folder is empty!");
                ImGui::SetWindowFontScale(1.0f);

                ImGui::EndChild();

                return;
            }

            ImVec2 m_dimensions = ImVec2(m_default_icon_size.x, m_default_icon_size.y);
            m_max_items_in_row = (int) (ImGui::GetColumnWidth() / m_default_icon_size.x);

            if (m_max_items_in_row == 0)
                m_max_items_in_row = 1;

            if (ImGui::BeginTable("Assets file view", m_max_items_in_row)) {
                int i = 0;
                for (const auto &entry: std::filesystem::directory_iterator(a_path)) {
                    if (i == m_max_items_in_row - 1) {
                        i = 0;
                        ImGui::TableNextRow();
                    }
                    std::string name = entry.path().filename().string();
                    ImGui::TableNextColumn();
                    ImGui::Image((void *) get_icon_texture_id(entry),
                                 m_dimensions,
                                 ImVec2(0.0f, 0.0f),
                                 ImVec2(1.0f, 1.0f),
                                 ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                                 ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
                    ImGui::TextWrapped("%s", name.c_str());
                    i++;
                }

                ImGui::EndTable();
            }
        }

        ImGui::EndChild();
    }

    void SceneEditorLayer::render_assets_popup(const std::filesystem::directory_entry &a_entry)
    {
        if (!m_mouse_move_mode_enabled && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
            ImGui::OpenPopup(a_entry.path().string().c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);

        if (ImGui::BeginPopup(a_entry.path().string().c_str())) {
            if (ImGui::MenuItem("New")) {
                auto new_path = a_entry.path();
                new_path /= "New folder";

                int next_id = 0;
                while (!std::filesystem::create_directory(new_path)) {
                    new_path = new_path.parent_path();
                    new_path /= "New folder";
                    new_path += "(" + std::to_string(next_id) + ")";
                    next_id++;
                }
            } else if (ImGui::MenuItem("Delete")) {
                std::filesystem::remove_all(a_entry.path());
            } else if (ImGui::MenuItem("Rename")) {
                LOG_INFO("Rename {0}", a_entry.path().filename().string().c_str());
            }

            ImGui::EndPopup();
        }
    }

    GLuint SceneEditorLayer::get_icon_texture_id(const std::filesystem::directory_entry &a_entry)
    {
        if (a_entry.is_directory()) {
            if (std::filesystem::is_empty(a_entry.path()))
                return m_icons.at("closed_dir")->get_id();
            else
                return m_icons.at("full_dir")->get_id();
        } else {
            std::string extension = a_entry.path().extension().string();

            if (m_icons.find(extension) != m_icons.end())
                return m_icons.at(extension)->get_id();
            else
                return m_icons.at("default")->get_id();
        }
    }

    void
    SceneEditorLayer::render_entry(const std::filesystem::directory_entry &a_entry, const ImVec2 &a_icon_dimensions)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        auto filename = a_entry.path().filename();

        if (a_entry.path().string() == m_selected_path)
            flags |= ImGuiTreeNodeFlags_Selected;

        if (std::filesystem::is_directory(a_entry.path())) {
            Texture *current_tex = m_icons.at("closed_dir");

            if (current_tex) {
                ImGui::Image((void *) current_tex->get_id(), a_icon_dimensions, (ImVec2 &&) current_tex->get_uv0(),
                             (ImVec2 &&) current_tex->get_uv1());
                ImGui::SameLine();
            }

            if (a_entry.path() != m_renamed_path) {
                if (ImGui::TreeNodeEx(filename.string().c_str(), flags)) {
                    if (ImGui::IsItemClicked()) {
                        m_selected_path = a_entry.path();
                    }

                    render_assets_popup(a_entry);
                    for (const auto &entry: std::filesystem::directory_iterator(a_entry.path())) {
                        render_entry(entry, a_icon_dimensions);
                    }

                    ImGui::TreePop();
                } else {
                    render_assets_popup(a_entry);

                    if (ImGui::IsItemClicked()) {
                        m_selected_path = a_entry.path().string();
                    }
                }
            } else {
                LOG_INFO("Renaming will be here");
            }
        }
    }

    void SceneEditorLayer::render_directories_buttons()
    {
        GLuint arrow_id = (m_icons.find("right_arrow") == m_icons.end()) ? m_icons.at("default")->get_id() : m_icons.at("right_arrow")->get_id();

        if (std::filesystem::exists(m_selected_path)) {
            std::vector<std::string> paths;
            std::filesystem::path current_path(m_selected_path);
            while (current_path != m_assets_path.parent_path() && !m_assets_path.empty()) {
                paths.push_back(current_path.filename().string());
                current_path = current_path.parent_path();
            }

            std::reverse(paths.begin(), paths.end());

            std::uint8_t index = 0;
            auto it = paths.begin();
            for (;it + 1 != paths.end(); it++, index++) {
                if (ImGui::Button(it->c_str())) {
                    set_path_from_buttons(m_selected_path, paths, index);
                    return;
                }

                ImGui::SameLine();
                ImGui::Image((void*)arrow_id, ImVec2(16.0f, 16.0f));

                if (index != paths.size() - 1) {
                    ImGui::SameLine();
                }
            }

            if (ImGui::Button(it->c_str())) {
                set_path_from_buttons(m_selected_path, paths, index);
            }
        }
    }

    void SceneEditorLayer::set_path_from_buttons(std::filesystem::path &path_to_set,
                                                 const std::vector<std::string> &a_to_set_from, std::uint8_t a_index)
    {
        if (a_to_set_from.empty()) {
            return;
        }

        std::filesystem::path temp("..\\res");

        std::uint8_t current_index = 0;
        for (auto it = a_to_set_from.begin();
             current_index <= a_index && it != a_to_set_from.end(); current_index++, it++) {
            temp /= *it;
        }

        path_to_set = temp;
    }

    void SceneEditorLayer::render_import_button()
    {
        if (ImGui::Button("Import")) {
            char *out_path = nullptr;
            nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &out_path);

            if(result == NFD_OKAY)
            {
                std::filesystem::path file_path(out_path);
                std::filesystem::path new_path(m_selected_path);

                try {
                    std::filesystem::copy(file_path, new_path /= file_path.filename());

                    if (new_path.extension() == ".obj") {
                        MeshManager::get()->load_mesh(new_path.string());
                    }
                }
                catch (std::exception &a_exception) {
                    LOG_ERROR("Failed to read file: {0}", a_exception.what());
                }
            }
        }
    }

    void SceneEditorLayer::move_camera(float dt)
    {
        if(GetKeyState('W') & 0x8000)
        {
            m_editor_camera->move(Direction::FRONT, dt);
        }
        if(GetKeyState('S') & 0x8000)
        {
            m_editor_camera->move(Direction::BACK, dt);
        }
        if(GetKeyState('A') & 0x8000)
        {
            m_editor_camera->move(Direction::LEFT, dt);
        }
        if(GetKeyState('D') & 0x8000)
        {
            m_editor_camera->move(Direction::RIGHT, dt);
        }
        if(GetKeyState('U') & 0x8000)
        {
            m_editor_camera->move(Direction::DOWN, dt);
        }
        if(GetKeyState('J') & 0x8000)
        {
            m_editor_camera->move(Direction::UP, dt);
        }
    }

    void SceneEditorLayer::render_menu_bar()
    {
        if(ImGui::BeginMainMenuBar())
        {
            ImGui::Image((void*)m_icons.at("right_arrow")->get_id(), ImVec2(16.0f, 16.0f));

            if(ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("XD");
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void SceneEditorLayer::render_transform_slider(const std::string& label,glm::vec3* data, float pos_to_add, float speed, float min)
    {
        float tag_padding = ImGui::GetWindowWidth() / 20.0f;
        float tag_slider_padding = ImGui::GetWindowWidth() / 2.5f;
        float slider_size = (ImGui::GetWindowWidth() - tag_padding - tag_slider_padding) / 7.0f;

        ImGui::SetCursorPosX(tag_padding);
        ImGui::Text("%s", label.c_str()); ImGui::SameLine();

        ImGui::PushItemWidth(slider_size);

        ImGui::SetCursorPosX(pos_to_add + tag_slider_padding + tag_padding);
        ImGui::Text("X"); ImGui::SameLine();
        ImGui::DragFloat(("##" + label + "x").c_str(), &(data->x), speed, min, FLT_MAX, "%.3f", ImGuiSliderFlags_None); ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + tag_padding / 2.0f);
        ImGui::Text("Y"); ImGui::SameLine();
        ImGui::DragFloat(("##" + label + "y").c_str(), &(data->y), speed, min, FLT_MAX, "%.3f", ImGuiSliderFlags_None); ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + tag_padding / 2.0f);
        ImGui::Text("Z"); ImGui::SameLine();
        ImGui::DragFloat(("##" + label + "z").c_str(), &(data->z), speed, min, FLT_MAX, "%.3f", ImGuiSliderFlags_None);

        ImGui::PopItemWidth();
    }
}