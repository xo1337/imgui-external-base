#include "source.hpp"
#include "fonts.hpp"
#include "menu.hpp"
#include "util.hpp"

/*
Make sure to compile on x64 DEBUG/RELEASE.
Enjoy! xo1337.

Credits:
- https://github.com/noteffex/ImGui-Loader-Base/ (External ImGui)
- https://github.com/JustasMasiulis/xorstr (JustasMasiulis, xorstr_)
*/

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::string random_title = util::rand_str(16);

    WNDCLASSEX window_class = { 
        sizeof(WNDCLASSEX), 
        CS_CLASSDC,
        [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)->LRESULT {
            if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
                return true;

            switch (msg)
            {
            case WM_SIZE:
                if (device_handle != 0 && wParam != SIZE_MINIMIZED)
                {
                    device_parameters.BackBufferWidth = LOWORD(lParam);
                    device_parameters.BackBufferHeight = HIWORD(lParam);
                    ResetDevice();
                }
                return 0;
            case WM_SYSCOMMAND:
                if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                    return 0;
                break;
            case WM_DESTROY:
                ExitProcess(0);
                return 0;
            }
            return DefWindowProcA(hWnd, msg, wParam, lParam);
        }, 
        0L,
        0L, 
        GetModuleHandleA(0), 
        0,
        0,
        0, 
        0, 
        random_title.c_str(),
        0 
    };

    RegisterClassExA(&window_class);
    window_handle = CreateWindowExA(0, window_class.lpszClassName, random_title.c_str(), WS_POPUP, 0, 0, 5, 5, 0, 0, window_class.hInstance, 0);

    if (!CreateDeviceD3D(window_handle))
    {
        CleanupDeviceD3D();
        UnregisterClassA(window_class.lpszClassName, window_class.hInstance);
        return 1;
    }

    ShowWindow(window_handle, SW_HIDE);
    UpdateWindow(window_handle);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 
   
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;

    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;

    ImFontConfig rubik;
    rubik.FontDataOwnedByAtlas = false;

    io.Fonts->AddFontFromMemoryTTF(const_cast<unsigned char*>(font_rubik), sizeof(font_rubik), 19.5, &rubik);
    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 32.5f, &icons_config, icons_ranges);
    io.Fonts->AddFontDefault();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    if (!Menu::Initialize())
    {
        CleanupDeviceD3D();
        UnregisterClassA(window_class.lpszClassName, window_class.hInstance);
        return 1;
    }

    ImGui_ImplWin32_Init(window_handle);
    ImGui_ImplDX9_Init(device_handle);

    while (true)
    {
        MSG message;
        if (PeekMessageA(&message, 0, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
            continue;
        }

        if (message.message == WM_QUIT)
            break;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
       
        Menu::Render();

        ImGui::EndFrame();

        device_handle->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (device_handle->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            device_handle->EndScene();
        }

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = device_handle->Present(0, 0, 0, 0);
        if (result == D3DERR_DEVICELOST && device_handle->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
    }

    return 0;
}
