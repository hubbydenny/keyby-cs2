module;
#include "../../includes.hpp"

export module render;

export namespace render {
	void rect(ImDrawList* draw, float left, float top, float right, float bottom, ImU32 color, float thickness = 1.f);
	void corner_box(ImDrawList* draw, float left, float top, float right, float bottom, ImU32 color, float thickness = 1.f);
	void text_centered(ImDrawList* draw, float x, float y, const char* text, ImU32 color);
	void health_bar(ImDrawList* draw, float left, float top, float bottom, float fraction, float width, ImU32 color, bool gradient);
}
