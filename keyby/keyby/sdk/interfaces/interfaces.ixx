module;
#include "../../includes.hpp"
#include <d3d11.h>

export module interfaces;

export import csgo_input;
export import input_sys;
export import schema;
export import memory;
export import error;


namespace interfaces {
    export error::Result<void> create();
    export void destroy() noexcept;
    export error::Result<void> create_d3d11_res(IDXGISwapChain* swap_chain);
    export void destroy_d3d11() noexcept;
    export inline inputsys* InputSystem = nullptr;
    export inline csgoinput* CsgoInput = nullptr;
    export inline i_schema_system* SchemaSystem = nullptr;

    export inline ID3D11Device* d3d11_device = nullptr;
    export inline ID3D11DeviceContext* d3d11_device_context = nullptr;
    export inline ID3D11RenderTargetView* d3d11_render_target_view = nullptr;
    export inline HWND hwnd = nullptr;
}
constexpr int CREATE_INTERFACE_OFFSET = 3;
constexpr int LIST_OFFSET = 7;

struct InterfaceReg {
    std::add_pointer_t<uintptr_t()> createFn;
    const char* name;
    InterfaceReg* next;
};

template <class T>
T* GetInterface(const char* moduleName, const char* interfaceName) {
    std::uintptr_t res = 0;

    HMODULE moduleBase = GetModuleHandleA(moduleName);
    if (!moduleBase) return nullptr;

    using CreateInterfaceFn = void* (*)(const char*, int*);
    const auto createInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleBase, "CreateInterface"));

    if (!createInterface) return nullptr;

    const uintptr_t list = reinterpret_cast<uintptr_t>(createInterface) + *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(createInterface) + CREATE_INTERFACE_OFFSET) + LIST_OFFSET;
    for (const InterfaceReg* current = *reinterpret_cast<InterfaceReg**>(list); current; current = current->next)
    {
        if (std::string_view(current->name).find(interfaceName) != std::string_view::npos)  res = current->createFn();
    }
    return reinterpret_cast<T*>(res);
}
