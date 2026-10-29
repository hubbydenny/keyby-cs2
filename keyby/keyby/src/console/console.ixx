module;
#include "../../includes.hpp"

export module console;

namespace con {
    FILE* output_stream = nullptr;
    export bool alloc() {
        if (!AllocConsole())   return false;
        if (freopen_s(&output_stream, "CONOUT$", "w", stdout) != 0) {
            FreeConsole();
            return false;
        }
        return true;
    }
    export DWORD free() noexcept {
        if (output_stream) std::fflush(output_stream);
        if (!FreeConsole()) return GetLastError();
        if (output_stream) {
            std::fclose(output_stream);
            output_stream = nullptr;
        }
        return ERROR_SUCCESS;
    }

    export template <class... Args>
        void log(std::format_string<Args...> fmt, Args&&... args) {
        std::println(fmt, std::forward<Args>(args)...);
        std::fflush(stdout);
    }
}
