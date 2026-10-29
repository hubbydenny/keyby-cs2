import visuals;
import globals;
import cfg;
import render;
#include "../../../includes.hpp"

namespace visuals {
	static ImU32 color(const std::array<float, 4>& value) {
		return ImGui::ColorConvertFloat4ToU32({ value[0], value[1], value[2], value[3] });
	}

	static void draw_box(ImDrawList* draw, float left, float top, float right, float bottom) {
		const auto box = color(config::visuals::box_color);
		const auto outline = color(config::visuals::outline_color);
		if (config::visuals::outline) {
			if (config::visuals::corner_box)
				render::corner_box(draw, left - 1.f, top - 1.f, right + 1.f, bottom + 1.f, outline, 2.f);
			else
				render::rect(draw, left - 1.f, top - 1.f, right + 1.f, bottom + 1.f, outline);
		}
		if (config::visuals::corner_box)
			render::corner_box(draw, left, top, right, bottom, box);
		else
			render::rect(draw, left, top, right, bottom, box);
	}

	static void draw_name(ImDrawList* draw, controller_t* controller, float x, float top) {
		if (!config::visuals::name) return;
		const char* name = controller->Name();
		if (name && *name) render::text_centered(draw, x, top - 2.f, name, color(config::visuals::name_color));
	}

	static void draw_health(ImDrawList* draw, player_t* player, float left, float top, float bottom) {
		if (config::visuals::health_bar)
			render::health_bar(draw, left, top, bottom, player->health() / 100.f,
				color(config::visuals::health_color), config::visuals::health_gradient);
	}

	void DrawBoxes(i_entity_list* entity_list, player_t* local) {
		if (!entity_list || !local) return;
		if (!config::visuals::drawesp) return;
		auto* draw = ImGui::GetBackgroundDrawList();
		for (int i = 0; i <= 64; ++i) {
			auto ctrl = entity_list->get_controller_by_index(i);
			if (!ctrl) continue;
			auto player = entity_list->get_player_from_controller(ctrl);
			if (!player || player == local || !player->IsAlive()) continue;
			if (player->team() == globals::local_player->team()) continue;

			vec3_t feet_world = player->abs_origin();
			vec3_t head_world = player->GetEyePosition();
			head_world.z += 6.f;

			vec3_t feet_screen{};
			vec3_t head_screen{};

			if (!math::world_to_screen(feet_world, feet_screen)) continue;
			if (!math::world_to_screen(head_world, head_screen)) continue;

			const auto height = feet_screen.y - head_screen.y + 0.4f;
			if (height <= 1.f) continue;
			const auto width = height * 0.45f;
			const auto left = head_screen.x - width * 0.5f;
			const auto right = head_screen.x + width * 0.5f;
			const auto top = head_screen.y + 0.5f;
			const auto bottom = feet_screen.y + 0.5f;
			draw_box(draw, left, top, right, bottom);
			draw_name(draw, ctrl, head_screen.x, top);
			draw_health(draw, player, left, top, bottom);
		}
	}
}
