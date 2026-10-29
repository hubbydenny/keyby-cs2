module;
#include "../../includes.hpp"

export module init;

export import error;
import interfaces;
import console;
import hooks;

export class cheat_init {
public:
    enum class stage { idle, console, interfaces, ready };

    cheat_init() = default;
    cheat_init(const cheat_init&) = delete;
    cheat_init& operator=(const cheat_init&) = delete;

    [[nodiscard]] error::Result<void> init() {
        if (step_ == stage::ready) return {};

        step_ = stage::console;
        if (!con::alloc()) {
            step_ = stage::idle;
            return std::unexpected(error::Info{
                error::Code::InitializationFailed, "Console initialization failed"});
        }
        owns_console_ = true;

        step_ = stage::interfaces;
        if (auto result = interfaces::create(); !result) {
            return std::unexpected(result.error());
        }
        con::log("[+] interfaces");
        if (auto result = hooks::start(); !result) return std::unexpected(result.error());
        con::log("[+] hooks");
        step_ = stage::ready;
        return {};
    }

    DWORD detach(HMODULE instance = nullptr, DWORD exit_code = 0) {
        if (!hooks::stop()) {
            con::log("[-] Failed to disable hooks");
            return ERROR_BUSY;
        }
        if (step_ == stage::interfaces || step_ == stage::ready) {
            interfaces::destroy();
            step_ = stage::console;
        }
        if (owns_console_) {
            if (const auto error = con::free(); error != ERROR_SUCCESS) {
                con::log("[-] FreeConsole failed: {}", error);
                return error;
            }
            owns_console_ = false;
        }
        step_ = stage::idle;
        if (instance) FreeLibraryAndExitThread(instance, exit_code);
        return ERROR_SUCCESS;
    }

    [[nodiscard]] stage current_stage() const noexcept { return step_; }
    static cheat_init SysAttr;

private:
    stage step_ = stage::idle;
    bool owns_console_ = false;
};

inline cheat_init cheat_init::SysAttr{};
