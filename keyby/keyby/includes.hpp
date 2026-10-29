#pragma once

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
#include <string>
#include <string_view>
#include <chrono>
#include <atomic>
#include <stdexcept>
#include <utility>
#include <expected>
#include <source_location>

#include "ext/safetyhook/safetyhook.hpp"
#include "ext/safetyhook/Zydis.h"

#include "ext/imgui/imgui.h"
#include "ext/imgui/imconfig.h"
#include "ext/imgui/imgui_impl_dx11.h"
#include "ext/imgui/imgui_impl_win32.h"
#include "ext/imgui/imgui_internal.h"
#include "ext/imgui/imstb_rectpack.h"
#include "ext/imgui/imstb_textedit.h"
#include "ext/imgui/imstb_truetype.h"
