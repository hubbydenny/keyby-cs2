module;
#include <atomic>
#include <array>

export module cfg;

export namespace config {
	export namespace menu {
		export std::atomic_bool opened{ false };
	}
	export namespace cheatutils {
		export std::atomic_bool allow_detach{ false };
	}
	export namespace visuals {
		export bool drawesp{ false };
		export bool corner_box{ false };
		export bool outline{ true };
		export bool name{ false };
		export bool health_bar{ false };
		export bool health_gradient{ true };
		export std::array<float, 4> box_color{ 1.f, 1.f, 1.f, 1.f };
		export std::array<float, 4> outline_color{ 0.f, 0.f, 0.f, 1.f };
		export std::array<float, 4> name_color{ 1.f, 1.f, 1.f, 1.f };
		export std::array<float, 4> health_color{ 0.f, 1.f, 0.f, 1.f };
	}
}
