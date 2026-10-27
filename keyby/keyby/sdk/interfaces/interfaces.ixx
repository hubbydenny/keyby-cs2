module;
export module interfaces;
#include "../../includes.hpp"

constexpr int CREATE_INTERFACE_OFFSET = 3;
constexpr int LIST_OFFSET = 7;

//https://hackvshack.net/threads/how-to-retrieve-interfaces-in-cs2.6349/

struct InterfaceReg {
    std::add_pointer_t<uintptr_t()> createFn;
    const char* name;
    InterfaceReg* next;
};

export std::uintptr_t GetInterface(const char* moduleName, const char* interfaceName) {
    std::uintptr_t res = 0;

    HMODULE moduleBase = GetModuleHandleA(moduleName);
    if (!moduleBase) return res;

    // Step 2: Locate the CreateInterface function.
    using CreateInterfaceFn = void* (*)(const char*, int*);
    const auto createInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleBase, "CreateInterface"));

    // Step 3: Locate the interface list and iterate through it to find the one we need.
    const uintptr_t list = reinterpret_cast<uintptr_t>(createInterface) + *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(createInterface) + CREATE_INTERFACE_OFFSET) + LIST_OFFSET;
    for (const InterfaceReg* current = *reinterpret_cast<InterfaceReg**>(list); current; current = current->next)
    {
        if (std::string_view(current->name).find(interfaceName) != std::string_view::npos)  res = current->createFn();
    }
    return res;
}