module;
#include "../../includes.hpp"

export module console;

namespace con {
    export bool alloc() {
        if (!AllocConsole())   return false;
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        return true;
    }
    export template <class... Args>
        void log(std::format_string<Args...> fmt, Args&&... args) {
        std::println(fmt, std::forward<Args>(args)...);
    }
    //TODO: log error handler 
    /* бред ебаный :lol:
    export template <typename EA>
    void log(std::format_string<Args...> fmt, Args&&... args) {
        if (!EA) std::println();
    }
    */
}