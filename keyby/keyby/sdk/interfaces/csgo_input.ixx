module;
#include "../../includes.hpp"
export module csgo_input;
import memory;
import patts;
export import class_user_cmd;
export import players_class;

export class csgoinput {
public:
    vec3_t* get_view_angles(std::int32_t slot = 0) {
        using function_t = vec3_t*(__fastcall*)(csgoinput*, std::int32_t);
        static auto fn = reinterpret_cast<function_t>(mem::find_pattern("client.dll", patts::GetViewAngles));
        return fn ? fn(this, slot) : nullptr;
    }

    bool set_view_angles(const vec3_t& angles, std::int32_t slot = 0) {
        using function_t = void(__fastcall*)(csgoinput*, std::int32_t, vec3_t&);
        static auto fn = reinterpret_cast<function_t>(mem::find_pattern("client.dll", patts::SetViewAngles));
        if (!fn) return false;
        auto copy = angles;
        fn(this, slot, copy);
        return true;
    }

    user_cmd_t* get_user_cmd(controller_t* controller) {
        const auto sequence = get_sequence_number(controller);
        if (!sequence) return nullptr;
        using function_t = user_cmd_t*(__fastcall*)(controller_t*, std::uint32_t);
        static auto fn = reinterpret_cast<function_t>(mem::find_pattern("client.dll", patts::GetUserCmdBySequence));
        return fn ? fn(controller, sequence) : nullptr;
    }

private:
    std::uint32_t get_sequence_number(controller_t* controller) {
        if (!controller) return 0;
        using tick_fn = void(__fastcall*)(controller_t*, std::int32_t*);
        using array_fn = void*(__fastcall*)(void**, std::int32_t);
        static auto get_tick = reinterpret_cast<tick_fn>(mem::find_pattern("client.dll", patts::GetUserCmdTick));
        static auto get_array = reinterpret_cast<array_fn>(mem::find_pattern("client.dll", patts::GetUserCmdArray));
        static void** first_array = [] {
            auto* address = mem::find_pattern("client.dll", patts::FirstUserCmdArray);
            return address ? static_cast<void**>(mem::read_pointer(mem::resolve_absolute_rip_address(address, 3, 7))) : nullptr;
        }();
        if (!get_tick || !get_array || !first_array) return 0;
        std::int32_t tick = 0;
        get_tick(controller, &tick);
        tick = tick == -1 ? -1 : tick - 1;
        auto* command_array = get_array(first_array, tick);
        if (!command_array) return 0;
        return *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(command_array) + 0x5910);
    }

};
