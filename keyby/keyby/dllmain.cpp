#include "includes.hpp"
import init;
import cfg;
import console;

auto __stdcall CheatThread(LPVOID instance) -> DWORD {
    auto& system = cheat_init::SysAttr;
    const auto result = system.init();

    if (!result) {
        con::log("[-] {}", result.error().message);
        OutputDebugStringA(result.error().message.data());
        return system.detach(reinterpret_cast<HMODULE>(instance), 1);
    }

    while ((GetAsyncKeyState(VK_F2) & 0x8000) == 0 && !config::cheatutils::allow_detach) {
        Sleep(100);
    }

    con::log("[+] detaching");
    return system.detach(reinterpret_cast<HMODULE>(instance));
}

auto DllMain(HINSTANCE instance, DWORD reason, LPVOID) -> BOOL {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);
        HANDLE thread = CreateThread(nullptr, 0, CheatThread, instance, 0, nullptr);
        if (!thread) return FALSE;
        CloseHandle(thread);
    }
    return TRUE;
}
