module;
#include "../../includes.hpp"

export module entitylist_interface;
import memory;
import players_class;
import patts;


export class i_entity_list {
public:
	void* get_base_entity(std::int32_t index)
	{
		using function_t = void* (__fastcall*)(i_entity_list*, std::int32_t);
		static function_t fn = reinterpret_cast<function_t>(mem::find_pattern("client.dll", patts::GetBaseEntity));

		if (!fn) return nullptr;
		return fn(this, index);
	}

	controller_t* get_controller_by_index(std::int32_t index)
	{
		return reinterpret_cast<controller_t*>(get_base_entity(index));
	}

	controller_t* get_local_controller()
	{
		using function_t = controller_t * (__fastcall*)(std::int32_t);
		static function_t fn = reinterpret_cast<function_t>(mem::resolve_absolute_rip_address(mem::find_pattern("client.dll", patts::GetLocalPlayerController), 1, 5));

		if (fn)
		{
			controller_t* controller = fn(-1);

			if (controller) return controller;
		}

		for (std::int32_t i = 1; i <= 64; i++)
		{
			controller_t* controller = get_controller_by_index(i);

			if (controller && controller->IsLocalPlrController()) return controller;
		}
		return nullptr;
	}

	player_t* get_player_from_controller(controller_t* controller)
	{
		if (!controller) return nullptr;
		const std::uint32_t pawn_handle = controller->PawnHandle();

		if (pawn_handle == 0xFFFFFFFF) return nullptr;
		const std::int32_t index = static_cast<std::int32_t>(pawn_handle & 0x7FFF);

		if (index <= 0) return nullptr;
		return reinterpret_cast<player_t*>(get_base_entity(index));
	}

	player_t* get_local_player()
	{
		return get_player_from_controller(get_local_controller());
	}

	void* get_entity_from_handle(entity_handle_t handle)
	{
		if (handle == invalid_entity_handle) return nullptr;
		return get_base_entity(static_cast<std::int32_t>(handle & 0x7FFF));
	}
};
