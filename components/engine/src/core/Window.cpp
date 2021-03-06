//
// Created by BlackFlage on 01.10.2021.
//

#include <dwmapi.h>
#include <chrono>
#include <thread>
#include <strsafe.h>
#include <windowsx.h>

#include "Window.h"
#include "Macro.h"
#include "Logger.h"
#include "GL/wglew.h"
#include "Application.h"

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

namespace vis
{
    bool Window::m_temp_window_created = false;
    bool Window::m_window_created = false;

    LRESULT CALLBACK wnd_proc_temp(HWND a_hwnd, UINT a_msg, WPARAM a_wparam, LPARAM a_lparam)
    {
        switch(a_msg)
        {
            case WM_CREATE:
                Window::m_temp_window_created = true;
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }

        return DefWindowProcA(a_hwnd, a_msg, a_wparam, a_lparam);
    }

    LRESULT CALLBACK win_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
        {
            case WM_KEYDOWN:
            {
                KeyPressEvent event((int)wParam);
                Application::get_instance()->on_event(event);

                break;
            }
            case WM_KEYUP:
            {
                KeyReleaseEvent event((int)wParam);
                Application::get_instance()->on_event(event);

                break;
            }
            case WM_CHAR:
            {
                CharInputEvent event((char)wParam);
                Application::get_instance()->on_event(event);

                break;
            }
            case WM_LBUTTONUP:
            {
                MouseButtonReleaseEvent event(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                INPUT->set_button_state(0, false);

                break;
            }
            case WM_RBUTTONUP:
            {
                MouseButtonReleaseEvent event(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                INPUT->set_button_state(1, false);

                break;
            }
            case WM_LBUTTONDOWN:
            {
                MouseButtonPressEvent event(MK_LBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                INPUT->set_button_state(0, true);

                break;
            }
            case WM_RBUTTONDOWN:
            {
                MouseButtonPressEvent event(MK_RBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                INPUT->set_button_state(1, true);

                break;
            }
            case WM_MOUSEMOVE:
            {
                POINT mouse_point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

                MouseMoveEvent event(mouse_point.x, mouse_point.y);
                Application::get_instance()->on_event(event);

                if(Application::get_window_instance() && !Application::get_window_instance()->get_show_cursor())
                {
                    POINT client_center = Application::get_window_instance()->get_client_center();
                    if(!SetCursorPos(client_center.x, client_center.y))
                    {
                        LOG_ERROR("Failed to set cursor position!");
                        break;
                    }
                }

                break;
            }
            case WM_MBUTTONUP:
            {
                MouseButtonReleaseEvent event(MK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                break;
            }
            case WM_MBUTTONDOWN:
            {
                MouseButtonPressEvent event(MK_MBUTTON, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                break;
            }
            case WM_XBUTTONDOWN:
            {
                MouseButtonPressEvent event(GET_XBUTTON_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                break;
            }
            case WM_XBUTTONUP:
            {
                MouseButtonReleaseEvent event(GET_XBUTTON_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                break;
            }
            case WM_MOUSEWHEEL:
            {
                MouseScrollEvent event(GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
                Application::get_instance()->on_event(event);
                break;
            }
            case WM_CLOSE:
            {
                Application::set_running(false);
                PostQuitMessage(APPLICATION_CLOSED);
                break;
            }
            case WM_SIZE:
            {
                if(wParam == SIZE_MAXIMIZED)
                {
                    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
                }

                if(wParam == SIZE_MINIMIZED)
                {
                    ShowWindow(hwnd, SW_SHOWMINIMIZED);
                    Application::set_minimized(true);
                    break;
                }

                RECT new_client_rect;

                if(Application::get_window_instance())
                {
                    GetClientRect(Application::get_window_instance()->get_context()->m_hwnd, &new_client_rect);

                    WindowResizeEvent* event = new WindowResizeEvent(new_client_rect);
                    Application::get_instance()->on_event(*event);
                    Application::set_resize_event(event);
                    Application::set_minimized(false);
                }

                break;
            }
            case WM_SETFOCUS:
            {
                WindowFocusEvent event(false);
                Application::get_instance()->on_event(event);

                break;
            }
            case WM_KILLFOCUS:
            {
                WindowFocusEvent event(true);
                Application::get_instance()->on_event(event);

                break;
            }
            default:
            {
                break;
            }
        }

        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }

    void retrieve_last_error(LPTSTR lpsz_function_name)
    {
        LPVOID lp_msg_buff;
        LPVOID lp_display_buff;
        DWORD dw = GetLastError();

        FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lp_msg_buff,
                0, nullptr );

        lp_display_buff = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCSTR)lp_msg_buff) + lstrlen((LPCSTR)lpsz_function_name) + 40) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lp_display_buff,
                        LocalSize(lp_display_buff) / sizeof(TCHAR),
                        TEXT("%s failed with error %d: %s"),
                        lpsz_function_name, dw, lp_msg_buff);

        MessageBox(nullptr, (LPCTSTR)lp_display_buff, TEXT("Error"), MB_OK);

        LocalFree(lp_msg_buff);
        LocalFree(lp_display_buff);
        ExitProcess(dw);
    }

    Window::Window(Context* a_context, Settings a_settings)
    : m_context(a_context), m_settings(a_settings), m_show_cursor(true)
    {

    }

    Window::~Window()
    {
        wglMakeCurrent(m_context->m_hdc, nullptr);
        wglDeleteContext(m_context->m_hglrc);
        ReleaseDC(m_context->m_hwnd, m_context->m_hdc);
        DestroyWindow(m_context->m_hwnd);

        delete m_context;
    }

    Window *Window::create_window(const Settings& a_settings)
    {
        if(!initialize_opengl())
        {
            LOG_ERROR("Failed to initialize OpenGL!");
            return nullptr;
        }

        Context* context = create_permanent_window(a_settings.m_width, a_settings.m_height, a_settings.m_name.c_str());
        if(!context)
        {
            LOG_ERROR("Failed to create Window Context!");
            return nullptr;
        }

        return new Window(context, a_settings);
    }

    std::optional<int> Window::pull_events()
    {
        MSG msg;

        while(PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
        {
            if(msg.message == WM_QUIT)
            {
                return msg.wParam;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return {};
    }

    bool Window::initialize_opengl()
    {
        HINSTANCE h_instance = GetModuleHandle(nullptr);

        WNDCLASSEX temp_wc;
        temp_wc.cbSize = sizeof(WNDCLASSEX);
        temp_wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
        temp_wc.lpfnWndProc = &wnd_proc_temp;
        temp_wc.cbClsExtra = 0;
        temp_wc.cbWndExtra = 0;
        temp_wc.hInstance = h_instance;
        temp_wc.hIcon = nullptr;
        temp_wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
        temp_wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        temp_wc.lpszMenuName = 0;
        temp_wc.lpszClassName = TEXT("oglTempClass");
        temp_wc.hIconSm = 0;

        ASSERT_B(RegisterClassEx(&temp_wc), "Failed to register temporary wnd class!");

        HWND temp_hwnd = CreateWindowEx(
                0,
                TEXT("oglTempClass"),
                TEXT("oglTempWindow"),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
                nullptr,
                nullptr,
                h_instance,
                nullptr);

        ASSERT_B(temp_hwnd, "Failed to create window handle!");

        MSG msg = {0};
        while(!m_temp_window_created && PeekMessageA(&msg, temp_hwnd, 0, 0, PM_NOYIELD | PM_REMOVE))
        {
            using namespace std::chrono_literals;

            DispatchMessageA(&msg);
            std::this_thread::sleep_for(100ms);
        }

        HDC temp_dc = GetDC(temp_hwnd);
        ASSERT_B(temp_dc, "Failed to get temporary context!");

        PIXELFORMATDESCRIPTOR pfd;
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cRedBits = 0;
        pfd.cRedShift = 0;
        pfd.cGreenBits = 0;
        pfd.cGreenShift = 0;
        pfd.cBlueBits = 0;
        pfd.cBlueShift = 0;
        pfd.cAlphaBits = 0;
        pfd.cAlphaShift = 0;
        pfd.cAccumBits = 0;
        pfd.cAccumRedBits = 0;
        pfd.cAccumGreenBits = 0;
        pfd.cAccumBlueBits = 0;
        pfd.cAccumAlphaBits = 0;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.cAuxBuffers = 0;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.bReserved = 0;
        pfd.dwLayerMask = 0;
        pfd.dwVisibleMask = 0;
        pfd.dwDamageMask = 0;

        int pixel_format = ChoosePixelFormat(temp_dc, &pfd);
        DescribePixelFormat(temp_dc, pixel_format, sizeof(pfd), &pfd);

        ASSERT_B(SetPixelFormat(temp_dc, pixel_format, &pfd), "Failed to set pixel format!");

        HGLRC temp_gl_context = wglCreateContext(temp_dc);
        wglMakeCurrent(temp_dc, temp_gl_context);

        if(glewInit() != GLEW_OK)
        {
            LOG_ERROR("Failed to initialize GLEW!");
            return false;
        }

        wglMakeCurrent(temp_dc, 0);
        wglDeleteContext(temp_gl_context);
        ReleaseDC(temp_hwnd, temp_dc);
        DestroyWindow(temp_hwnd);

        return true;
    }

    Context* Window::create_permanent_window(int a_width, int a_height, const char* a_name)
    {
        HINSTANCE h_instance = GetModuleHandle(nullptr);

        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
        wc.lpfnWndProc = win_proc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = h_instance;
        wc.hIcon = 0;
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = 0;
        wc.lpszClassName = TEXT("OpenGLWindow");
        wc.hIconSm = 0;

        ASSERT(RegisterClassEx(&wc), "Failed to register window class!");

        HWND hwnd = CreateWindowEx(
                0,
                "OpenGLWindow",
                a_name,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, a_width, a_height,
                0, 0, h_instance, 0
                );

        MSG msg = {0};
        while(!m_window_created && PeekMessageA(&msg, hwnd, 0, 0, PM_NOYIELD | PM_REMOVE))
        {
            using namespace std::chrono_literals;

            DispatchMessageA(&msg);
            std::this_thread::sleep_for(100ms);
        }

        HDC dc = GetDC(hwnd);
        ASSERT(dc, "Failed to retrieve device context from window!");

        static const int pixel_attribs[] ={
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, 24,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,
                WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
                WGL_SAMPLES_ARB, 4,
                0
        };

        int pixel_format = 0;
        unsigned int num_pixel_format = 0;
        wglChoosePixelFormatARB(dc, pixel_attribs, nullptr, 1, &pixel_format, &num_pixel_format);

        PIXELFORMATDESCRIPTOR pfd2;
        DescribePixelFormat(dc, pixel_format, sizeof(pfd2), &pfd2);
        ASSERT(SetPixelFormat(dc, pixel_format, &pfd2), "Failed to set pixel format in permanent window!");

        static const int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_LAYER_PLANE_ARB, 0,
                WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0
        };

        HGLRC gl_context = wglCreateContextAttribsARB(dc, nullptr, attribs);
        ASSERT(gl_context, "Failed to create OpenGL context!");

        SetFocus(hwnd);

        return new Context(hwnd, dc, gl_context);
    }

    RECT Window::get_client_rect() const
    {
        RECT client_area;
        GetClientRect(m_context->m_hwnd, &client_area);

        return client_area;
    }

    POINT Window::get_client_center() const
    {
        RECT client_area = get_client_rect();
        POINT client_center = { client_area.right / 2, client_area.bottom / 2 };

        return client_center;
    }

    POINT Window::get_mapped_client_center() const
    {
        POINT mapped_client_center = get_client_center();
        ClientToScreen(m_context->m_hwnd, &mapped_client_center);

        return mapped_client_center;
    }

    void Window::set_show_cursor(bool a_show_cursor)
    {
        m_show_cursor = a_show_cursor;

        ShowCursor(a_show_cursor);
    }
}