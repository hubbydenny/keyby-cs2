module;
#include "../../includes.hpp"

module render;

namespace render {
	void rect(ImDrawList* draw, float left, float top, float right, float bottom, ImU32 color, float thickness) {
		if (draw) draw->AddRect({ left, top }, { right, bottom }, color, 0.f, 0, thickness);
	}

	void corner_box(ImDrawList* draw, float left, float top, float right, float bottom, ImU32 color, float thickness) {
		if (!draw) return;
		const float w = (right - left) * .25f;
		const float h = (bottom - top) * .25f;
		draw->AddLine({ left, top }, { left + w, top }, color, thickness);
		draw->AddLine({ left, top }, { left, top + h }, color, thickness);
		draw->AddLine({ right - w, top }, { right, top }, color, thickness);
		draw->AddLine({ right, top }, { right, top + h }, color, thickness);
		draw->AddLine({ left, bottom - h }, { left, bottom }, color, thickness);
		draw->AddLine({ left, bottom }, { left + w, bottom }, color, thickness);
		draw->AddLine({ right - w, bottom }, { right, bottom }, color, thickness);
		draw->AddLine({ right, bottom - h }, { right, bottom }, color, thickness);
	}

	void text_centered(ImDrawList* draw, float x, float y, const char* text, ImU32 color) {
		if (!draw || !text || !*text) return;
		const ImVec2 size = ImGui::CalcTextSize(text);
		draw->AddText({ x - size.x * .5f, y - size.y }, color, text);
	}

	void health_bar(ImDrawList* draw, float left, float top, float bottom, float fraction, ImU32 color, bool gradient) {
		if (!draw) return;
		fraction = (std::clamp)(fraction, 0.f, 1.f);
		draw->AddRect({ left - 7.f, top - 1.f }, { left - 2.f, bottom + 1.f }, IM_COL32(0, 0, 0, 255), 0.f, 0, 1.f);
		draw->AddRectFilled({ left - 6.f, top }, { left - 3.f, bottom }, IM_COL32(15, 15, 15, 220));
		const ImU32 fill = gradient ? static_cast<ImU32>(ImColor::HSV(fraction * .33f, 1.f, 1.f)) : color;
		draw->AddRectFilled({ left - 6.f, bottom - (bottom - top) * fraction }, { left - 3.f, bottom }, fill);
	}
}
