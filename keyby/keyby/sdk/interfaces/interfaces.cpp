module interfaces;
import csgo_input;
import input_sys;
import swapchain_interface;
import memory;
import error;
import console;

namespace interfaces {

    error::Result<void> create() {
        InputSystem = GetInterface<inputsys>("inputsystem.dll","InputSystemVersion001");

        if (!InputSystem) {
            con::log("[-] Failed interface: InputSystem, line: {}", __LINE__);
            return std::unexpected(error::Info { error::Code::InterfaceNotFound,"InputSystem initialization failed" });
        }
        return {};

    }

    void destroy() {}
}