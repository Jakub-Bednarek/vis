//
// Created by BlackFlage on 02.12.2021.
//

#include "layers/EntityComponentSystemLayer.h"
#include <optional>
#include <string>

#include "ImGui/imgui.h"
#include "Types.h"

namespace vis
{
    MainManager* main_manager = new MainManager();

    void EntityComponentSystemLayer::on_attach()
    {
        main_manager->init();

        register_components();
        register_systems();
        set_signatures();

        m_resources_manager = ResourcesManager::get_instance();
        m_component_names = {"Transform", "Color", "Mesh", "RigidBody", "Camera"};
        m_default_models_path = R"(C:\Users\BlackFlage\OneDrive - Politechnika Wroclawska\C++\visual\components\engine\res\models\default\)";

        m_camera = new Camera(glm::vec3(1.0f, 1.0f, 1.0f));
        m_shader = Shader::create_shader_name("vertex.glsl", "fragmentNoTex.glsl");

        Renderer::set_camera(m_camera);
        Renderer::set_shader(m_shader);
    }

    void EntityComponentSystemLayer::on_detach()
    {

    }

    void EntityComponentSystemLayer::on_event(Event &a_event)
    {
        EventDispatcher dispatcher(a_event);
        dispatcher.dispatch<WindowResizeEvent>([this](auto&& event) { on_window_resize_event(std::forward<decltype(event)>(event)); });
        dispatcher.dispatch<MouseMoveEvent>([this](auto&& event) { on_mouse_move_event(std::forward<decltype(event)>(event)); });
    }

    void EntityComponentSystemLayer::on_update(float a_delta_time)
    {
        m_physics_system->on_update(a_delta_time);
        m_entity_track_system->on_update(a_delta_time);
    }

    void EntityComponentSystemLayer::on_render()
    {
        m_renderer_system->on_render();
    }

    void EntityComponentSystemLayer::on_imgui_render()
    {
        std::uint16_t id = main_manager->get_current_entity();

        ImGui::Begin("Properties");

        if(id != MAX_ENTITIES)
        {
            show_transform_component(id);
            show_color_component(id);
            show_mesh_component(id);

            show_add_component_button(id);
        }

        ImGui::End();

        bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    void EntityComponentSystemLayer::register_components()
    {
        main_manager->register_component<Transform>();
        main_manager->register_component<Color>();
        main_manager->register_component<RigidBody>();
        main_manager->register_component<MeshComponent>();
    }

    void EntityComponentSystemLayer::register_systems()
    {
        m_physics_system = main_manager->register_system<PhysicsSystem>();
        m_renderer_system = main_manager->register_system<RendererSystem>();
        m_entity_track_system = main_manager->register_system<EntityTrackSystem>();
    }

    void EntityComponentSystemLayer::set_signatures()
    {
        {
            Signature phys_signature;
            phys_signature.set(main_manager->get_component_type<Transform>(), true);
            phys_signature.set(main_manager->get_component_type<RigidBody>(), true);
            main_manager->set_system_signature<PhysicsSystem>(phys_signature);
        }

        {
            Signature rend_signature;
            rend_signature.set(main_manager->get_component_type<MeshComponent>(), true);
            rend_signature.set(main_manager->get_component_type<Color>(), true);
            rend_signature.set(main_manager->get_component_type<Transform>(), true);
            main_manager->set_system_signature<RendererSystem>(rend_signature);
        }

        {
            Signature ent_signature;
            ent_signature.set(main_manager->get_component_type<Transform>(), true);
            main_manager->set_system_signature<EntityTrackSystem>(ent_signature);
        }
    }

    void EntityComponentSystemLayer::on_mouse_move_event(MouseMoveEvent &a_event)
    {

    }

    void EntityComponentSystemLayer::on_window_resize_event(WindowResizeEvent& a_event)
    {
        m_camera->recalculate_perspective(a_event.get_width(), a_event.get_height());
    }

    void EntityComponentSystemLayer::show_transform_component(std::uint16_t a_id)
    {
        Signature sig = main_manager->get_entity_signature(a_id);

        if(sig[main_manager->get_component_type<Transform>()])
        {
            if(ImGui::CollapsingHeader("Transform")) {
                auto& transform = main_manager->get_component<Transform>(a_id);

                //Position
                ImGui::DragFloat3("Position", &transform.m_position[0], 0.1f);

                //Rotation
                ImGui::DragFloat3("Rotation", &transform.m_rotation[0]);

                //Scale
                ImGui::DragFloat3("Scale", &transform.m_scale[0], 0.01f, 0.0f);
            }
        }
    }

    void EntityComponentSystemLayer::show_color_component(std::uint16_t a_id)
    {
        Signature sig = main_manager->get_entity_signature(a_id);

        if(sig[main_manager->get_component_type<Color>()])
        {
            if(ImGui::CollapsingHeader("Color"))
            {
                auto& color = main_manager->get_component<Color>(a_id);

                ImGui::ColorEdit3("Mesh color", &color.m_color[0]);
            }
        }
    }

    void EntityComponentSystemLayer::show_mesh_component(std::uint16_t a_id)
    {
        Signature sig = main_manager->get_entity_signature(a_id);

        if(sig[main_manager->get_component_type<MeshComponent>()])
        {
            if(ImGui::CollapsingHeader("Mesh"))
            {
                auto mesh_id = main_manager->get_component<MeshComponent>(a_id).m_id;
                auto mesh = m_resources_manager->get_mesh(mesh_id);

                ImGui::Text("Mesh id: %d", mesh_id);
                ImGui::Text("Vertices: %d", (int)mesh->get_vertices().size());
                ImGui::Text("Indices: %d", (int)mesh->get_indices().size());
            }
        }
    }

    void EntityComponentSystemLayer::show_add_component_button(std::uint16_t a_id)
    {
        ImVec2 button_size = ImVec2(-FLT_MIN, ImGui::GetWindowHeight() * 0.025f);

        if(ImGui::Button("Add Component", button_size))
            ImGui::OpenPopup("Components list");

        if(ImGui::BeginPopup("Components list"))
        {
            for(int i = 0; i < m_component_names.size(); i++)
            {
                if(ImGui::MenuItem(m_component_names.at(i)))
                {
                    add_component(a_id, m_component_names.at(i));
                }
            }

            ImGui::EndPopup();
        }
    }

    void EntityComponentSystemLayer::add_component(std::uint16_t a_id, const char *a_component_name)
    {
        if(std::strcmp(a_component_name, "Transform") == 0)
        {
            main_manager->add_component(a_id, Transform{
                .m_position = glm::vec3(0.0f),
                .m_rotation = glm::vec3(1.0f),
                .m_scale = glm::vec3(1.0f)
            });
        }
        else if(std::strcmp(a_component_name, "Color") == 0)
        {
            main_manager->add_component(a_id, Color{ .m_color = glm::vec3(0.3f) });
        }
        else if(std::strcmp(a_component_name, "Mesh") == 0)
        {
            MeshComponent mesh_comp = {.m_id = m_resources_manager->load_mesh(m_default_models_path + "cube.obj")};

            if(mesh_comp.m_id != MAX_COMPONENTS)
            {
                main_manager->add_component(a_id, mesh_comp);
            }
        }
        else if(std::strcmp(a_component_name, "RigidBody") == 0)
        {
            main_manager->add_component(a_id, RigidBody{ .vel_x = 0.0f, .vel_y = 0.0f, .vel_z = 0.0f});
        }
    }
}