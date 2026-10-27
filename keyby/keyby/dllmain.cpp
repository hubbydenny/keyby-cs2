#include "includes.hpp"
import interfaces;
import cfg;
import console;

DWORD WINAPI CheatThread(LPVOID instance) {
	con::alloc();
	con::log("[+] injected");
	interfaces::create();
	con::log("[+] interfaces");


	while (!GetAsyncKeyState(VK_F2) & 1 && !config::cfgutils::allow_detach) {
		Sleep(100);
	}

	con::log("[+] detaching");
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(instance), 0);
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		HANDLE thread = CreateThread(nullptr, 0, CheatThread, instance, 0, nullptr);
		if (thread != INVALID_HANDLE_VALUE) {
			CloseHandle(thread);
			return TRUE;
		}
		return FALSE;
	}
}	