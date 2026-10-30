module;
#include "../../includes.hpp"

export module trace_class;
import memory;
import patts;
export import math;
export import players_class;

export {
    inline constexpr std::uint8_t trace_layer_default = 4;
    inline constexpr std::uint8_t trace_filter_type = 7;
    inline constexpr std::int32_t collision_group_player_movement = 11;

    inline constexpr std::uint64_t contents_solid = 1ull << 0;
    inline constexpr std::uint64_t contents_hitbox = 1ull << 1;
    inline constexpr std::uint64_t contents_window = 1ull << 8;
    inline constexpr std::uint64_t contents_pass_bullets = 1ull << 13;
    inline constexpr std::uint64_t contents_player = 1ull << 15;
    inline constexpr std::uint64_t contents_npc = 1ull << 16;
    inline constexpr std::uint64_t contents_debris = 1ull << 17;
    inline constexpr std::uint64_t contents_block_los = 1ull << 6;

    inline constexpr std::uint64_t mask_shot = contents_solid | contents_player | contents_npc |
        contents_window | contents_debris | contents_hitbox;
    inline constexpr std::uint64_t mask_player_visible = contents_solid | contents_window |
        contents_player | contents_npc | contents_debris | contents_hitbox | contents_block_los;

    struct ray_t {
        vec3_t mins{};
        vec3_t maxs{};
        char pad1[0x10]{};
        std::uint8_t type{};
        char pad2[0x7]{};
    };
    static_assert(offsetof(ray_t, type) == 0x28);
    static_assert(sizeof(ray_t) == 0x30);

    struct trace_filter_t { std::byte data[0x48]{}; };
    static_assert(sizeof(trace_filter_t) == 0x48);

    struct game_trace_t {
        void* surface{};
        player_t* player{};
        void* hitbox{};
        char pad1[0x38]{};
        std::uint32_t surface_flags{};
        char pad2[0x24]{};
        vec3_t startpos{};
        vec3_t endpos{};
        vec3_t normal{};
        char pad3[0xC]{};
        char pad4[0x4]{};
        float fraction{ 1.f };
        char pad5[0x6]{};
        bool all_solid{};
        char pad6[0x4]{};
        bool start_solid{};
        char pad7[0x48]{};

        bool hit() const { return start_solid || all_solid || fraction < 1.f; }
        bool clear() const { return !hit(); }
    };
    static_assert(offsetof(game_trace_t, fraction) == 0xAC);
    static_assert(sizeof(game_trace_t) == 0x108);

    class trace_t {
        void* manager_{ nullptr };
        using trace_shape_fn = bool(__fastcall*)(void*, ray_t*, vec3_t&, vec3_t&, trace_filter_t*, game_trace_t*);
        using filter_init_fn = void(__fastcall*)(trace_filter_t*, void*, std::uint64_t, std::int32_t, std::int32_t);

        static trace_shape_fn trace_shape() noexcept {
            static auto fn = reinterpret_cast<trace_shape_fn>(mem::find_pattern("client.dll", patts::TraceShape));
            return fn;
        }
        static filter_init_fn filter_init() noexcept {
            static auto fn = reinterpret_cast<filter_init_fn>(mem::find_pattern("client.dll", patts::TraceFilterInit));
            return fn;
        }

    public:
        bool initialize() noexcept {
            auto* address = mem::find_pattern("client.dll", patts::TraceManager);
            if (!address) return false;
            manager_ = static_cast<void*>(mem::read_pointer(mem::resolve_absolute_rip_address(address, 3, 7)));
            return manager_ && trace_shape() && filter_init();
        }
        void reset() noexcept { manager_ = nullptr; }
        bool ready() const noexcept { return manager_ && trace_shape() && filter_init(); }

        bool make_filter(trace_filter_t& filter, void* skip, std::uint64_t mask = mask_player_visible,
            std::int32_t layer = trace_layer_default) const noexcept {
            if (!filter_init()) return false;
            filter_init()(&filter, skip, mask, layer, trace_filter_type);
            return true;
        }

        bool line(const vec3_t& from, const vec3_t& to, void* skip, game_trace_t& result,
            std::uint64_t mask = mask_player_visible) const noexcept {
            if (!ready()) return false;
            trace_filter_t filter{};
            if (!make_filter(filter, skip, mask)) return false;
            ray_t ray{};
            vec3_t start = from;
            vec3_t end = to;
            return trace_shape()(manager_, &ray, start, end, &filter, &result);
        }
    };
}
