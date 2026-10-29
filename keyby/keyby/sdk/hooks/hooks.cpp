module;
#include "../../includes.hpp"
#include <dxgi.h>

module hooks;
import memory;
import interfaces;
import console;
import patts;
import menu;
import input_sys;
import csgo_input;
import globals;
import players_class;
import viewsetup_class;
import cfg;

namespace {
    safetyhook::InlineHook present_hook, resize_hook;
    using Present = HRESULT (STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT);
    using Resize = HRESULT (STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
    using RelativeMouse = void* (*)(inputsys*, bool);
    using MouseEnabled = bool (*)(csgoinput*);
    using OverrideView = void(__fastcall*)(void*, CViewSetup*);

    safetyhook::InlineHook relative_mouse_hook, mouse_enabled_hook, override_view_hook;

    OverrideView original_override_view = nullptr;
    RelativeMouse original_relative_mouse = nullptr;
    MouseEnabled original_mouse_enabled = nullptr;
    Present original_present = nullptr;
    Resize original_resize = nullptr;

    std::recursive_mutex render_mutex;
    std::atomic_uint active_calls = 0;

    std::atomic_bool stopping = false;
    IDXGISwapChain* current_chain = nullptr;
    bool attempted = false;
    std::recursive_mutex mouse_mutex;
    bool relative_mode_known = false;
    bool mouse_overridden = false;
    bool requested_relative_mode = false;

    constexpr std::size_t mouse_enabled_index = 15;
    constexpr std::size_t relative_mouse_index = 76;
    struct active_call {
        active_call() { ++active_calls; }
        ~active_call() { --active_calls; }
    };

    HRESULT STDMETHODCALLTYPE present(IDXGISwapChain* chain, UINT interval, UINT flags) {
        active_call call;
        std::lock_guard lock(render_mutex);
        if (!stopping && !attempted && !(flags & DXGI_PRESENT_TEST)) {
            attempted = true;
            current_chain = chain;

            try {
                if (auto result = interfaces::create_d3d11_res(chain); !result)
                    con::log("[-] {}", result.error().message);
                else if (auto result = menu::create(); !result)
                    con::log("[-] {}", result.error().message);
            } catch (const std::exception& exception) {
                con::log("[-] {}", exception.what());
            }
        }
        if (!stopping && chain == current_chain && !(flags & DXGI_PRESENT_TEST)) menu::render();
        return original_present(chain, interval, flags);
    }

    HRESULT STDMETHODCALLTYPE resize(IDXGISwapChain* chain, UINT count, UINT width, UINT height, DXGI_FORMAT format, UINT flags) {
        active_call call;
        std::lock_guard lock(render_mutex);
        if (!stopping && chain == current_chain) {
            if (!menu::destroy()) return DXGI_ERROR_INVALID_CALL;
            interfaces::destroy_d3d11();
            attempted = false;
        }
        return original_resize(chain, count, width, height, format, flags);
    }
    bool mouse_enabled(csgoinput* input) {
        active_call call;
        return !stopping && config::menu::opened ? false : original_mouse_enabled(input);
    }

    void __fastcall override_view(void* client_mode, CViewSetup* view_setup) {
        active_call call;
        if (!original_override_view) return;
        original_override_view(client_mode, view_setup);
        if (view_setup) view_setup->fov = config::visuals::ViewSetup_fov;
    }
    void* relative_mouse(inputsys* input, bool enabled) {
        active_call call;
        std::lock_guard lock(mouse_mutex);
        requested_relative_mode = enabled;
        relative_mode_known = true;
        return original_relative_mouse(input, !stopping && config::menu::opened ? false : enabled);
    }

    void update_mouse_mode(bool opened) {
        std::lock_guard lock(mouse_mutex);
        if (!original_relative_mouse || !interfaces::InputSystem) return;
        if (opened) {
            if (mouse_overridden) return;
            if (!relative_mode_known)
                requested_relative_mode = original_mouse_enabled(interfaces::CsgoInput);
            mouse_overridden = true;
            original_relative_mouse(interfaces::InputSystem, false);
        }
        else if (mouse_overridden) {
            mouse_overridden = false;
            original_relative_mouse(interfaces::InputSystem, requested_relative_mode);
        }
    }
}

error::Result<void> hooks::start() try {
    if (present_hook) return {};
    for (int i = 0; i < 500 && !GetModuleHandleA("gameoverlayrenderer64.dll"); ++i) Sleep(10);
    auto* present_target = mem::find_pattern("gameoverlayrenderer64.dll", patts::PresentHookTarget);
    auto* resize_target = mem::find_pattern("gameoverlayrenderer64.dll", patts::ResizeTarget);
    auto* ovrd_view_setup = mem::find_pattern("client.dll", patts::OverrideView);
    if (!interfaces::CsgoInput || !interfaces::InputSystem) return std::unexpected(error::Info{error::Code::InterfaceNotFound, "Mouse input interfaces are null"});

    auto* mouse_vtable = static_cast<void**>(mem::read_pointer(interfaces::CsgoInput));
    auto* relative_vtable = static_cast<void**>(mem::read_pointer(interfaces::InputSystem));
    auto* mouse_target = mem::read_pointer(mouse_vtable + mouse_enabled_index);
    auto* relative_target = mem::read_pointer(relative_vtable + relative_mouse_index);

    override_view_hook = safetyhook::create_inline(ovrd_view_setup, override_view, safetyhook::InlineHook::StartDisabled);
    present_hook = safetyhook::create_inline(present_target, present, safetyhook::InlineHook::StartDisabled);
    resize_hook = safetyhook::create_inline(resize_target, resize, safetyhook::InlineHook::StartDisabled);
    mouse_enabled_hook = safetyhook::create_inline(mouse_target, mouse_enabled, safetyhook::InlineHook::StartDisabled);
    relative_mouse_hook = safetyhook::create_inline(relative_target, relative_mouse, safetyhook::InlineHook::StartDisabled);


    if (!present_hook || !resize_hook || !mouse_enabled_hook || !relative_mouse_hook || !override_view_hook) {
        stop();
        return std::unexpected(error::Info{error::Code::InitializationFailed, "Failed to create DXGI hooks"});
    }
    original_present = present_hook.original<Present>();
    original_resize = resize_hook.original<Resize>();
    original_mouse_enabled = mouse_enabled_hook.original<MouseEnabled>();
    original_relative_mouse = relative_mouse_hook.original<RelativeMouse>();
    original_override_view = override_view_hook.original<OverrideView>();

    relative_mode_known = false;
    mouse_overridden = false;
    menu::set_mouse_mode_handler(update_mouse_mode);
    stopping = false;
    attempted = false;

    if (!mouse_enabled_hook.enable() || !relative_mouse_hook.enable() || !resize_hook.enable() ||
        !present_hook.enable() || !override_view_hook.enable()) {
        stop();
        return std::unexpected(error::Info{error::Code::InitializationFailed, "Failed to enable DXGI hooks"});
    }
    return {};
} catch (const std::exception& exception) {
    con::log("[-] {}", exception.what());
    stop();
    return std::unexpected(error::Info{error::Code::InitializationFailed, "Dxgi hook failed"});
}

bool hooks::stop() {
    stopping = true;
    for (auto* hook : {&present_hook, &resize_hook, &mouse_enabled_hook, &relative_mouse_hook, &override_view_hook})
        if (*hook && !hook->disable()) return false;
    while (active_calls.load() != 0) Sleep(1);
    std::lock_guard lock(render_mutex);
    if (!menu::destroy()) return false;
    menu::set_mouse_mode_handler(nullptr);
    config::menu::opened = false;
    for (auto* hook : {&present_hook, &resize_hook, &mouse_enabled_hook, &relative_mouse_hook, &override_view_hook})
        hook->reset();
    original_mouse_enabled = nullptr;
    original_relative_mouse = nullptr;
    original_override_view = nullptr;
    current_chain = nullptr;
    attempted = false;
    return true;
}
