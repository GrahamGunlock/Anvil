#include "Gui.h"

#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "Themes/Themes.h"

#include "../Main/Settings.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

bool Config::Gui::Toggle = false;

namespace Gui
{
    using WNDPROC = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

    WNDPROC oWndProc;

    LRESULT CALLBACK hkWndProc(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (Config::Gui::Toggle)
        {
            ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
            return true;
        }
        return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);

    }

    //===== ==========================================================================

    void Initialize(IDirect3DDevice9 * pDevice)
    {
        static bool IsGuiInitialized = false;
        if (IsGuiInitialized == false)
        {
            IsGuiInitialized = true;

            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();

            io.MouseDrawCursor = true;
            io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

            ImGui_ImplWin32_Init(FindWindowA(NULL, "Counter-Strike: Global Offensive"));
            ImGui_ImplDX9_Init(pDevice);

            oWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, "Counter-Strike: Global Offensive"), GWL_WNDPROC, (LONG_PTR)hkWndProc);

            LoadDefaultTheme();
        }
    }

    void Render(IDirect3DDevice9 * pDevice)
    {
        if ((GetAsyncKeyState(VK_ESCAPE) & 1) && Config::Gui::Toggle)
            Config::Gui::Toggle = 0;

        if (GetAsyncKeyState(VK_INSERT) & 1) 
            Config::Gui::Toggle = !Config::Gui::Toggle;

        if (!Config::Gui::Toggle)
            return;

        DWORD dwOld_D3DRS_COLORWRITEENABLE;  // Allows us to draw without net_graph
        pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);  // Allows us to draw without net_graph
        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);  // Allows us to draw without net_graph

        // Start Frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Begin Window

        ImGui::Begin("Anvil");

        ImGui::Checkbox("Bhop", &Config::Bunnyhop::Enable);

        ImGui::End();

        // End Frame
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

        pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);  // Allows us to draw without net_graph
    }
}

