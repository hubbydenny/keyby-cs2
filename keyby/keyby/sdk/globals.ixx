module;
export module globals;

export import entitylist_interface;
export import players_class;
export import trace_class;
export import class_user_cmd;

export namespace globals {
	export inline i_entity_list* entity_list = nullptr;
	export inline controller_t* local_controller = nullptr;
	export inline player_t* local = nullptr;
	export inline player_t*& local_player = local;
	export inline trace_t trace{};
	export inline user_cmd_t* cmd = nullptr;
	export inline user_cmd_t* silent_cmd = nullptr;
	export inline vec3_t silent_camera{};
	export inline bool silent_active = false;

	export inline void update_local() {
		if (!entity_list) {
			local_controller = nullptr;
			local = nullptr;
			cmd = nullptr;
			return;
		}
		local_controller = entity_list->get_local_controller();
		local = entity_list->get_player_from_controller(local_controller);
	}
}
