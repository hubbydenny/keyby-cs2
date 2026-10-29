module;
#include "../../includes.hpp"
#include <wrl/client.h>
#include <d3d11.h>
#include <dxgi.h>

module interfaces;
import csgo_input;
import input_sys;
import schema;
import entitylist_interface;
import globals;
import patts;

import memory;
import error;
import console;


error::Result<void> interfaces::create_d3d11_res(IDXGISwapChain* swap_chain) {
    using namespace interfaces;
    if (!swap_chain) return std::unexpected(error::Info{error::Code::InterfaceNotFound, "SwapChain is null"});
    if (d3d11_device) return {};
    Microsoft::WRL::ComPtr<ID3D11Device> device;
    if (FAILED(swap_chain->GetDevice(IID_PPV_ARGS(device.GetAddressOf()))) || !device) return std::unexpected(error::Info{error::Code::InitializationFailed, "Failed to get D3D11 device"});

    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
    device->GetImmediateContext(context.GetAddressOf());
    if (!context) return std::unexpected(error::Info{error::Code::InitializationFailed, "D3D11 context is null"});

    Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
    if (FAILED(swap_chain->GetBuffer(0, IID_PPV_ARGS(back_buffer.GetAddressOf()))) || !back_buffer)
        return std::unexpected(error::Info{error::Code::InitializationFailed, "Failed to get swap chain back buffer"});

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target;
    if (FAILED(device->CreateRenderTargetView(back_buffer.Get(), nullptr, render_target.GetAddressOf())) || !render_target)
        return std::unexpected(error::Info{error::Code::InitializationFailed, "Failed to create render target view"});

    DXGI_SWAP_CHAIN_DESC desc{};
    if (FAILED(swap_chain->GetDesc(&desc)) || !desc.OutputWindow)
        return std::unexpected(error::Info{error::Code::InitializationFailed, "Failed to get swap chain window"});

    d3d11_device = device.Detach();
    d3d11_device_context = context.Detach();
    d3d11_render_target_view = render_target.Detach();
    hwnd = desc.OutputWindow;
    return {};
}

void interfaces::destroy_d3d11() noexcept {
    if (interfaces::d3d11_render_target_view) std::exchange(interfaces::d3d11_render_target_view, nullptr)->Release();
    if (interfaces::d3d11_device_context) std::exchange(interfaces::d3d11_device_context, nullptr)->Release();
    if (interfaces::d3d11_device) std::exchange(interfaces::d3d11_device, nullptr)->Release();
    interfaces::hwnd = nullptr;
}

namespace interfaces {
    error::Result<void> create() try {
        std::uint8_t* address = mem::find_pattern("client.dll", "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 F6"); // .text:0000000180CFA432                 mov     rcx, cs:qword_18279D0A0
        interfaces::CsgoInput = static_cast<csgoinput*>(mem::read_pointer(mem::resolve_absolute_rip_address(address, 3, 7)));
        if (!CsgoInput) {
            return std::unexpected(error::Info{
            error::Code::InterfaceNotFound, "[-] Failed interface: csgoinput" });
        }
        
        interfaces::InputSystem = GetInterface<inputsys>("inputsystem.dll","InputSystemVersion001");
        if (!InputSystem) {
            con::log("[-] Failed interface: InputSystem, line: {}", __LINE__);
            return std::unexpected(error::Info { error::Code::InterfaceNotFound,"InputSystem initialization failed" });
        }

        interfaces::SchemaSystem = GetInterface<i_schema_system>("schemasystem.dll", "SchemaSystem_001");
        if (!SchemaSystem) {
            con::log("[-] Failed interface: SchemaSystem, line: {}", __LINE__);
            return std::unexpected(error::Info{ error::Code::InterfaceNotFound,"SchemaSystem initialization failed" });
        }

        auto* entity_address = mem::resolve_absolute_rip_address(
            mem::find_pattern("client.dll", patts::EntityList), 3, 7);
        globals::entity_list = reinterpret_cast<i_entity_list*>(mem::read_pointer(entity_address));
        if (!globals::entity_list)
            return std::unexpected(error::Info{error::Code::InterfaceNotFound, "EntityList initialization failed"});

        schema_system::bind(SchemaSystem);
        return {};
    } catch (const std::exception& exception) {
        con::log("[-] {}", exception.what());
        return std::unexpected(error::Info{error::Code::InitializationFailed, "Interface initialization failed"});
    }

    void destroy() noexcept {
        destroy_d3d11();
        InputSystem = nullptr;
        CsgoInput = nullptr;
        globals::entity_list = nullptr;
        globals::local_controller = nullptr;
        globals::local_player = nullptr;
        schema_system::bind(nullptr);
        SchemaSystem = nullptr;
    }
}
