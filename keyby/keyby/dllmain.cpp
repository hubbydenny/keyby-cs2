#include "includes.hpp"
import cfg;

DWORD WINAPI CheatThread(LPVOID instance) {
	while (!GetAsyncKeyState(VK_F2) && !config::cfgutils::allow_detach) {
		Sleep(100);
		MessageBox(nullptr, L"Injected", L"keyby", MB_ICONHAND);
	}
	//con::log("[+] Detaching");
	MessageBox(nullptr, L"Detached", L"keyby", MB_ICONHAND);
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(instance), 0);
	return 0;
}
BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);
	}
	//std::thread cheatthr();
	HANDLE thread = CreateThread(nullptr, 0, CheatThread, instance, 0, nullptr);
	if (thread != INVALID_HANDLE_VALUE) {
		CloseHandle(thread);
		return TRUE;
	}
	return FALSE;
}	