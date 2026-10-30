module;
#include "../../includes.hpp"

export module menu_style;

export namespace menu_style {
    inline void apply() {
        auto& style = ImGui::GetStyle();
        style.WindowRounding = 0.f;
        style.ChildRounding = 0.f;
        style.PopupRounding = 0.f;
        style.FrameRounding = 2.f;
        style.GrabRounding = 2.f;
        style.ScrollbarRounding = 0.f;
        style.WindowBorderSize = 1.f;
        style.ChildBorderSize = 1.f;
        style.FrameBorderSize = 1.f;
        style.TabBorderSize = 1.f;
        style.WindowPadding = { 14.f, 12.f };
        style.FramePadding = { 8.f, 5.f };
        style.ItemSpacing = { 8.f, 8.f };
        style.ItemInnerSpacing = { 6.f, 4.f };

        auto& colors = style.Colors;
        colors[ImGuiCol_WindowBg] = { .035f, .04f, .05f, 1.f };
        colors[ImGuiCol_ChildBg] = { .045f, .05f, .06f, 1.f };
        colors[ImGuiCol_Border] = { .28f, .30f, .34f, 1.f };
        colors[ImGuiCol_FrameBg] = { .08f, .09f, .11f, 1.f };
        colors[ImGuiCol_FrameBgHovered] = { .12f, .13f, .16f, 1.f };
        colors[ImGuiCol_FrameBgActive] = { .15f, .16f, .19f, 1.f };
        colors[ImGuiCol_CheckMark] = { .95f, .55f, .16f, 1.f };
        colors[ImGuiCol_SliderGrab] = { .95f, .55f, .16f, 1.f };
        colors[ImGuiCol_SliderGrabActive] = { 1.f, .68f, .24f, 1.f };
        colors[ImGuiCol_Header] = { .12f, .13f, .16f, 1.f };
        colors[ImGuiCol_HeaderHovered] = { .16f, .17f, .20f, 1.f };
        colors[ImGuiCol_HeaderActive] = { .20f, .21f, .24f, 1.f };
        colors[ImGuiCol_Button] = { .10f, .11f, .13f, 1.f };
        colors[ImGuiCol_ButtonHovered] = { .15f, .16f, .19f, 1.f };
        colors[ImGuiCol_ButtonActive] = { .20f, .21f, .24f, 1.f };
        colors[ImGuiCol_Tab] = { .08f, .09f, .11f, 1.f };
        colors[ImGuiCol_TabHovered] = { .16f, .17f, .20f, 1.f };
        colors[ImGuiCol_Separator] = { .20f, .21f, .24f, 1.f };
    }
}
