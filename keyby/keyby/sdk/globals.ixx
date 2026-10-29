module;
export module globals;

export import entitylist_interface;
export import players_class;

export namespace globals {
	export inline i_entity_list* entity_list = nullptr;
	export inline controller_t* local_controller = nullptr;
	export inline player_t* local_player = nullptr;

	export inline void update_local() {
		if (!entity_list) {
			local_controller = nullptr;
			local_player = nullptr;
			return;
		}
		local_controller = entity_list->get_local_controller();
		local_player = entity_list->get_player_from_controller(local_controller);
	}
}
