module;
#include "../../../includes.hpp"

module movement;
import cfg;
import globals;

namespace movement {
    bool Bunnyhop(user_cmd_t* cmd) {
        if (!config::movement::bhop || !cmd || !globals::local || !globals::local->IsAlive()) return false;
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000) == 0) return false;
        cmd->buttons |= in_jump;
        cmd->sync_buttons();
        cmd->update_move_crc();
        return true;
    }
}
