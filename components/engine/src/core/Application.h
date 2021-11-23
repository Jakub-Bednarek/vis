//
// Created by BlackFlage on 01.10.2021.
//
#pragma once
#ifndef VISUAL_APPLICATION_H
#define VISUAL_APPLICATION_H

#include "Window.h"
#include <functional>
#include "LayerStack.h"
#include "Timer.h"
#include "event/Event.h"
#include "event/KeyboardEvent.h"
#include "event/WindowEvent.h"
#include "event/MouseEvent.h"
#include <iostream>
#include "Shader.h"
#include "Input.h"

#define INPUT Application::get_input_instance()

namespace vis
{
    class Application
    {
    public:
        ~Application();

        //App run management
        void run();
        void initialize();

        void on_event(Event& a_event);
        void on_update();
        void on_render();

        void push_layer(Layer* a_layer);
        void detach_layer(Layer* a_layer);

        void recalculate_refresh_interval();
        void set_refresh_interval(int a_refresh_rate);

        inline static Input* get_input_instance() { return m_input; }
        inline static Window* get_window_instance() { return m_window; }
        inline static Application* get_instance() { return m_instance; }
        inline static bool is_running() { return m_running; }

        inline static void set_running(bool a_running) { m_running = a_running; }
        inline double get_delta_time() { return m_timer.get_delta_time(); }
        inline double get_time_passed() { return m_timer.get_time_passed();}

        static Application* create_instance();
        static void set_resize_event(WindowResizeEvent* a_event);
    private:
        Application();

        void start_opengl_thread();
        void update_input_data(MouseMoveEvent& a_event);

        static DWORD WINAPI opengl_thread(LPVOID a_param);
        static void on_resize_event(WindowResizeEvent& a_event);
    private:
        static Window* m_window;
        static Application* m_instance;
        static Input* m_input;
        static bool m_running;
        static bool m_send_move_event;
        static bool m_opengl_initialized;
        static WindowResizeEvent* m_resize_event;

        Timer other;
        Timer m_timer;
        LayerStack m_layer_stack;

        double m_refresh_rate = 120.0; //frames per second
        double m_refresh_interval;

        static bool m_gl_context_should_resize;
    };
}


#endif //VISUAL_APPLICATION_H
