module;
#include "../../includes.hpp"
#include <algorithm>
module memory;

namespace mem {
    bool is_readable(const void* address, std::size_t size) {
        auto cursor = reinterpret_cast<std::uintptr_t>(address);
        if (!cursor || size > UINTPTR_MAX - cursor) return false;
        const auto end = cursor + size;
        while (cursor < end) {
            MEMORY_BASIC_INFORMATION region{};
            if (!VirtualQuery(reinterpret_cast<void*>(cursor), &region, sizeof(region)) ||
                region.State != MEM_COMMIT || (region.Protect & (PAGE_NOACCESS | PAGE_GUARD))) return false;
            const auto next = reinterpret_cast<std::uintptr_t>(region.BaseAddress) + region.RegionSize;
            if (next <= cursor) return false;
            cursor = next;
        }
        return true;
    }
    static std::vector<int> ida_pattern_to_bytes(const char* pattern) {
        std::vector<int> bytes;
        while (*pattern) {
            if (*pattern == ' ') { ++pattern; continue; }
            if (*pattern == '?') {
                bytes.push_back(-1);
                if (*++pattern == '?') ++pattern;
                continue;
            }
            char* end = nullptr;
            const auto value = std::strtoul(pattern, &end, 16);
            if (end == pattern || value > 0xFF) throw std::runtime_error("Invalid pattern");
            bytes.push_back(static_cast<int>(value));
            pattern = end;
        }
        return bytes;
    }

    void* read_pointer(const void* address) {
        void* value = nullptr;
        if (!ReadProcessMemory(GetCurrentProcess(), address, &value, sizeof(value), nullptr))
            throw std::runtime_error("Cannot read interface pointer; verify signature and offset");
        return value;
    }

    std::uint8_t* find_pattern(const char* module_name, const char* pattern) {
        const auto module = GetModuleHandleA(module_name);
        if (!module) throw std::runtime_error(std::format("Module not loaded: {}", module_name));
        const auto base = reinterpret_cast<std::uintptr_t>(module);
        const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(module);
        const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
        const auto image_end = base + nt->OptionalHeader.SizeOfImage;
        const auto bytes = ida_pattern_to_bytes(pattern);
        if (bytes.empty()) throw std::runtime_error("Empty pattern");

        for (auto cursor = base; cursor < image_end;) {
            MEMORY_BASIC_INFORMATION region{};
            if (!VirtualQuery(reinterpret_cast<void*>(cursor), &region, sizeof(region))) break;
            const auto end = (std::min)(image_end, reinterpret_cast<std::uintptr_t>(region.BaseAddress) + region.RegionSize);
            if (end <= cursor) break;
            if (region.State == MEM_COMMIT && !(region.Protect & (PAGE_NOACCESS | PAGE_GUARD))) {
                std::vector<std::uint8_t> data(end - cursor);
                SIZE_T read = 0;
                ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(cursor), data.data(), data.size(), &read);
                for (std::size_t i = 0; read >= bytes.size() && i <= read - bytes.size(); ++i) {
                    std::size_t j = 0;
                    while (j < bytes.size() && (bytes[j] == -1 || data[i + j] == bytes[j])) ++j;
                    if (j == bytes.size()) return reinterpret_cast<std::uint8_t*>(cursor + i);
                }
            }
            cursor = end;
        }
        throw std::runtime_error(std::format("Pattern not found in {}: {}", module_name, pattern));
    }

    std::uint8_t* resolve_absolute_rip_address(std::uint8_t* instr, std::size_t offset, std::size_t size) {
        std::int32_t displacement;
        if (!ReadProcessMemory(GetCurrentProcess(), instr + offset, &displacement, sizeof(displacement), nullptr))
            throw std::runtime_error("Cannot read RIP displacement");
        return reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(instr) + size + displacement);
    }
}
