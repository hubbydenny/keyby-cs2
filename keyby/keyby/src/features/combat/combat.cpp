module;
#include "../../../includes.hpp"

module combat;
import cfg;
import globals;
import interfaces;
import math;
import types;

namespace combat {
    bool Aimbot() {
        if (!config::combat::enabled || !globals::cmd || !globals::cmd->valid() ||!globals::entity_list || !globals::local || !globals::local_controller || !globals::local->IsAlive()) return false;

        auto* view = globals::cmd->get_view_angles();
        if (!view) return false;

        auto* input_view = interfaces::CsgoInput ? interfaces::CsgoInput->get_view_angles() : nullptr;
        const vec3_t camera_angles = input_view ? *input_view : view->angles;

        const vec3_t eye = globals::local->GetEyePosition();
        const vec3_t current = camera_angles;
        player_t* best = nullptr;
        vec3_t best_delta{};
        float best_fov = config::combat::fov;

        for (auto index = 1; index <= 64; ++index) {
            auto* controller = globals::entity_list->get_controller_by_index(index);
            if (!controller) continue;
            auto* player = globals::entity_list->get_player_from_controller(controller);
            if (!player || player == globals::local || !player->IsAlive()) continue;
            if (!config::cheatutils::allow_team) if (player->team() == globals::local->team()) continue;

            const vec3_t target = player->GetBonePosition(static_cast<std::int32_t>(boneindex::head_0));
            if (target.is_zero()) continue;

            game_trace_t trace{};
            if (!globals::trace.line(eye, target, globals::local, trace, mask_shot)) continue;
            if (trace.hit() && trace.player != player) continue;

            const vec3_t delta = math::calculate_angle(eye, target, current);
            const float fov = std::hypot(delta.x, delta.y);
            if (fov > best_fov) continue;

            best = player;
            best_delta = delta;
            best_fov = fov;
        }

        if (!best) return false;

        const float smooth = (std::max)(1.f, config::combat::smoothvalue);
        vec3_t next = current + best_delta * (1.f / smooth);
        normalize_angles(next);
        globals::cmd->set_view_angles(next);
        const bool history_updated = globals::cmd->set_input_history(next, globals::local_controller, globals::local, true);
        const bool crc_updated = globals::cmd->update_move_crc();
        if (input_view) interfaces::CsgoInput->set_view_angles(config::combat::silent ? camera_angles : next);
        if (config::combat::silent && input_view) {
            globals::silent_cmd = globals::cmd;
            globals::silent_camera = camera_angles;
            globals::silent_active = true;
        }
        return history_updated && crc_updated;
    }
}
