#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

HWND                     window_handle;
LPDIRECT3DDEVICE9        device_handle;
D3DPRESENT_PARAMETERS    device_parameters;
LPDIRECT3D9              device_direct;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

bool CreateDeviceD3D(HWND hWnd)
{
    if ((device_direct = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    memset(&device_parameters, 0, sizeof(device_parameters));

    device_parameters.Windowed = TRUE;
    device_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    device_parameters.BackBufferFormat = D3DFMT_UNKNOWN;
    device_parameters.EnableAutoDepthStencil = TRUE;
    device_parameters.AutoDepthStencilFormat = D3DFMT_D16;
    device_parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    
    if (device_direct->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &device_parameters, &device_handle) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (device_handle) 
    { 
        device_handle->Release();
        device_handle = 0; 
    }

    if (device_direct)
    {
        device_direct->Release(); 
        device_direct = 0; 
    }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT Result = device_handle->Reset(&device_parameters);
    if (Result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}