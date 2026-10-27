#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>
#include <unordered_map>
#include <deque>
#include <thread>
#include <mutex>
#include <span>
#include <print>
#include <thread>
#include <string>
#include <string_view>

#include "ext/imgui/imgui.h"
#include "ext/imgui/imconfig.h"
#include "ext/imgui/imgui_impl_dx11.h"
#include "ext/imgui/imgui_impl_win32.h"
#include "ext/imgui/imgui_internal.h"
#include "ext/imgui/imstb_rectpack.h"
#include "ext/imgui/imstb_textedit.h"
#include "ext/imgui/imstb_truetype.h"

#include "ext/minhook/buffer.h"
#include "ext/minhook/MinHook.h"
#include "ext/minhook/trampoline.h"
#include "ext/minhook/hde/pstdint.h"