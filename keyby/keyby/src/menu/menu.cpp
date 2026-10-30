module;
#include "../../includes.hpp"
#include <d3d11.h>
#include <wrl/client.h>

module menu;
import cfg;
import interfaces;
import globals;
import visuals;
import menu_style;

extern "C++" {
    IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
}

namespace {
    std::recursive_mutex menu_mutex;
    WNDPROC original_wnd = nullptr;
    HWND window = nullptr;
    bool initialized = false;
    bool click = false;
    void (*mouse_mode_handler)(bool) = nullptr;

    void set_open(bool opened) {
        config::menu::opened = opened;
        if (mouse_mode_handler) mouse_mode_handler(opened);
    }

    LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        {
            std::lock_guard lock(menu_mutex);
            if (initialized) {
                if (msg == WM_KEYDOWN && wparam == VK_INSERT && !(lparam & (1LL << 30)))
                    set_open(!config::menu::opened.load());
                ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
                if (config::menu::opened) {
                    switch (msg) {
                    case WM_INPUT: return DefWindowProcA(hwnd, msg, wparam, lparam);
                    case WM_MOUSEMOVE: case WM_LBUTTONDOWN: case WM_LBUTTONUP:
                    case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_MBUTTONDOWN: case WM_MBUTTONUP:
                    case WM_XBUTTONDOWN: case WM_XBUTTONUP: case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
                    case WM_KEYDOWN: case WM_KEYUP: case WM_SYSKEYDOWN: case WM_SYSKEYUP: case WM_CHAR:
                        return 0;
                    }
                }
            }
        }
        return CallWindowProcA(original_wnd, hwnd, msg, wparam, lparam);
    }
}

error::Result<void> menu::create() {
    std::lock_guard lock(menu_mutex);
    if (initialized) return {};
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();
    menu_style::apply();
    if (!ImGui_ImplWin32_Init(interfaces::hwnd)) {
        ImGui::DestroyContext();
        return std::unexpected(error::Info{error::Code::InitializationFailed, "ImGui Win32 initialization failed"});
    }
    if (!ImGui_ImplDX11_Init(interfaces::d3d11_device, interfaces::d3d11_device_context)) {
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        return std::unexpected(error::Info{error::Code::InitializationFailed, "ImGui DX11 initialization failed"});
    }
    window = interfaces::hwnd;
    SetLastError(ERROR_SUCCESS);
    original_wnd = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(window_proc)));
    if (!original_wnd) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        window = nullptr;
        return std::unexpected(error::Info{error::Code::InitializationFailed, "Failed to install menu WndProc"});
    }
    initialized = true;
    if (mouse_mode_handler && config::menu::opened) mouse_mode_handler(true);
    return {};
}

bool menu::destroy() {
    std::lock_guard lock(menu_mutex);
    if (!initialized) return true;
    if (IsWindow(window)) {
        if (reinterpret_cast<WNDPROC>(GetWindowLongPtrA(window, GWLP_WNDPROC)) != window_proc) return false;
        SetLastError(ERROR_SUCCESS);
        if (!SetWindowLongPtrA(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(original_wnd)) && GetLastError()) return false;
    }
    if (mouse_mode_handler) mouse_mode_handler(false);
    initialized = false;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    window = nullptr;
    return true;
}

void menu::render() {
    std::lock_guard lock(menu_mutex);
    if (!initialized || !interfaces::d3d11_render_target_view) return;
    auto& io = ImGui::GetIO();
    bool opened = config::menu::opened.load();

    if (opened)
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
    else
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    io.MouseDrawCursor = false;

    ImGui::NewFrame();
    globals::update_local();
    visuals::DrawBoxes();

    if (opened) {
        ImGui::SetNextWindowSize(ImVec2(430, 390), ImGuiCond_FirstUseEver);
        constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        if (ImGui::Begin("keyby", &opened, window_flags)) {
            constexpr ImGuiTabBarFlags tab_flags = ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_FittingPolicyResizeDown;
            if (ImGui::BeginTabBar("##tabs", tab_flags)) {

                if (ImGui::BeginTabItem("combat")) {
                    if (ImGui::BeginChild("##combat_panel", {}, true, ImGuiWindowFlags_NoScrollbar)) {
                        ImGui::Checkbox("enabled", &config::combat::enabled);
                        if (config::combat::enabled) {
                            ImGui::Checkbox("silent", &config::combat::silent);
                            ImGui::SliderFloat("fov", &config::combat::fov, 1.f, 30.f, "%.1f");
                            ImGui::SliderFloat("smooth", &config::combat::smoothvalue, 1.f, 20.f, "%.1f");
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("visuals")) {

                    if (ImGui::BeginChild("##visuals_panel", {}, true, ImGuiWindowFlags_NoScrollbar)) {
                        ImGui::SliderFloat("view fov", &config::visuals::ViewSetup_fov, 60.f, 150.f, "%.0f");
                        ImGui::Checkbox("boxes", &config::visuals::drawesp);
                        if (config::visuals::drawesp) {
                            ImGui::Indent(8.f);
                            ImGui::Checkbox("Team", &config::cheatutils::allow_team);
                            ImGui::Checkbox("corner box", &config::visuals::corner_box);
                            ImGui::Checkbox("outline", &config::visuals::outline);
                            ImGui::Checkbox("name", &config::visuals::name);
                            ImGui::Checkbox("health bar", &config::visuals::health_bar);
                            if (config::visuals::health_bar) {
                                ImGui::Checkbox("health gradient", &config::visuals::health_gradient);
                                ImGui::SliderFloat("health width", &config::visuals::health_bar_width, 1.f, 12.f, "%.1f");
                            }
                            ImGui::ColorEdit4("box color", config::visuals::box_color.data(), ImGuiColorEditFlags_NoInputs);
                            ImGui::ColorEdit4("outline color", config::visuals::outline_color.data(), ImGuiColorEditFlags_NoInputs);
                            if (config::visuals::name)
                                ImGui::ColorEdit4("name color", config::visuals::name_color.data(), ImGuiColorEditFlags_NoInputs);
                            if (config::visuals::health_bar)
                                ImGui::ColorEdit4("health color", config::visuals::health_color.data(), ImGuiColorEditFlags_NoInputs);
                            ImGui::Unindent(8.f);
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("movement")) {
                    if (ImGui::BeginChild("##movement_panel", {}, true, ImGuiWindowFlags_NoScrollbar)) {
                        ImGui::Checkbox("bhop", &config::movement::bhop);
                        ImGui::EndChild();
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();
        if (!opened) set_open(false);
    }

    ImGui::Render();
    auto* draw_data = ImGui::GetDrawData();
    if (draw_data->CmdListsCount == 0) return;
    auto* context = interfaces::d3d11_device_context;
    ID3D11RenderTargetView* targets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth;
    context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets, depth.GetAddressOf());
    context->OMSetRenderTargets(1, &interfaces::d3d11_render_target_view, nullptr);
    ImGui_ImplDX11_RenderDrawData(draw_data);
    context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets, depth.Get());
    for (auto* target : targets) if (target) target->Release();
}

void menu::set_mouse_mode_handler(void (*handler)(bool)) {
    std::lock_guard lock(menu_mutex);
    mouse_mode_handler = handler;
}
