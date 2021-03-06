//
// Created by BlackFlage on 01.10.2021.
//

#pragma once

#ifndef VISUAL_WINDOW_H
#define VISUAL_WINDOW_H

#include <Windows.h>
#include <optional>
#include <functional>
#include <utility>
#include "event/Event.h"

#define APPLICATION_CLOSED 333

namespace vis
{
    struct Context
    {
        Context(HWND a_hwnd, HDC a_hdc, HGLRC a_hglrc)
        :   m_hwnd(a_hwnd), m_hdc(a_hdc), m_hglrc(a_hglrc) {}

        HWND m_hwnd;
        HDC m_hdc;
        HGLRC m_hglrc;
    };

    struct Settings
    {
        Settings(int a_width, int a_height, std::string a_name)
        :   m_width(a_width), m_height(a_height), m_name(a_name) {}
        int m_width;
        int m_height;
        std::string m_name;
    };

    class Window
    {
    public:
        ~Window();

        static Window* create_window(const Settings& a_settings);

        inline void show_window() const { ShowWindow(m_context->m_hwnd, 5); }
        inline void hide_window() const { ShowWindow(m_context->m_hwnd,  0); }
        inline void show_fullscreen() const { ShowWindow(m_context->m_hwnd, 3); }
        inline void set_title(const char* a_title) const { SetWindowTextA(m_context->m_hwnd, a_title); }
        inline Context* get_context() const { return m_context; }
        inline int get_width() const { return m_settings.m_width; }
        inline int get_height() const { return m_settings.m_height; }
        inline bool get_show_cursor() const { return m_show_cursor; }

        RECT get_client_rect() const;
        POINT get_client_center() const;
        POINT get_mapped_client_center() const;

        void set_show_cursor(bool a_show_cursor);

        std::optional<int> pull_events();
    private:
        Window(Context* a_context, Settings a_settings);
        static bool initialize_opengl();
        static Context* create_permanent_window(int a_width, int a_height, const char* a_name);
    private:
        Context* m_context;
        Settings m_settings;

        bool m_show_cursor;
    public:
        static bool m_temp_window_created;
        static bool m_window_created;

        std::function<LRESULT(HWND, UINT, LPARAM, WPARAM)> m_win_proc;
    };
}


#endif //VISUAL_WINDOW_H
