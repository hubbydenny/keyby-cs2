module;
#include "../../includes.hpp"

export module class_user_cmd;
import memory;
import patts;
export import math;
export import types;
export import players_class;

export {
	class QAngle {
	public:
		char pad1[0x18];
		vec3_t angles;
	};

	class cin_button_state_pb_t {
	public:
		char pad1[0x10];
		std::uint32_t has_bits;
		std::int32_t cached_size;
		std::uint64_t buttonstate1;
		std::uint64_t buttons2;
		std::uint64_t buttons3;
	};

	class protobuf_repeated_ptr_field_t {
	public:
		struct rep_t {
			std::int32_t allocated_size;
			void* elements[1];
		};

		void* arena;
		std::int32_t current_size;
		std::int32_t capacity;
		rep_t* rep;
	};

	class utl_string_t {
	public:
		char* memory;
		std::int32_t allocation_count;
		std::int32_t grow_size;
		std::int32_t length;
		char pad[0x4];
	};

	class subtick_move_step_t {
	public:
		enum has_bits_t : std::uint32_t
		{
			has_button = 1u << 0,
			has_pressed = 1u << 1,
			has_when = 1u << 2,
			has_analog_forward_delta = 1u << 3,
			has_analog_left_delta = 1u << 4,
			has_pitch_delta = 1u << 5,
			has_yaw_delta = 1u << 6
		};

		char pad1[0x10];
		std::uint32_t has_bits;
		std::int32_t cached_size;
		std::uint64_t button;
		bool pressed;
		char pad2[0x3];
		float when;
		float analog_forward_delta;
		float analog_left_delta;
		float pitch_delta;
		float yaw_delta;

		void set_button(std::uint64_t value)
		{
			has_bits |= has_button;
			button = value;
		}

		void set_pressed(bool value)
		{
			has_bits |= has_pressed;
			pressed = value;
		}

		void set_when(float value)
		{
			has_bits |= has_when;
			when = value;
		}

		void set_analog_forward_delta(float value)
		{
			has_bits |= has_analog_forward_delta;
			analog_forward_delta = value;
		}

		void set_analog_left_delta(float value)
		{
			has_bits |= has_analog_left_delta;
			analog_left_delta = value;
		}

		void set_pitch_delta(float value)
		{
			has_bits |= has_pitch_delta;
			pitch_delta = value;
		}

		void set_yaw_delta(float value)
		{
			has_bits |= has_yaw_delta;
			yaw_delta = value;
		}

		bool has_movement() const
		{
			return (has_bits & (has_button | has_analog_forward_delta | has_analog_left_delta | has_pitch_delta | has_yaw_delta)) != 0;
		}
	};

	class csgo_interpolation_info_pb_t {
	public:
		char pad1[0x10];
		std::uint32_t has_bits;
		std::int32_t cached_size;
		float frac;
		std::int32_t src_tick;
		std::int32_t dst_tick;
		char pad2[0x4];
	};


	class csgo_input_history_entry_pb_t {
	public:
		enum has_bits_t : std::uint32_t {
			has_viewangles = 1u << 0,
			has_render_tick_count = 1u << 9,
			has_render_tick_fraction = 1u << 10,
			has_player_tick_count = 1u << 11,
			has_player_tick_fraction = 1u << 12
		};

		char pad1[0x10];
		std::uint32_t has_bits;
		std::int32_t cached_size;
		QAngle* view_angles;
		void* cl_interp;
		csgo_interpolation_info_pb_t* sv_interp0;
		csgo_interpolation_info_pb_t* sv_interp1;
		csgo_interpolation_info_pb_t* player_interp;
		void* shoot_position;
		void* target_head_pos_check;
		void* target_abs_pos_check;
		QAngle* target_abs_ang_check;
		std::int32_t render_tick_count;
		float render_tick_fraction;
		std::int32_t player_tick_count;
		float player_tick_fraction;
		std::int32_t frame_number;
		std::int32_t target_ent_index;
	};

	class cbase_user_cmd_pb_t {
	public:
		enum has_bits_t : std::uint32_t {
			has_move_crc = 0x1,
			has_buttons = 0x2,
			has_viewangles = 0x4,
			has_legacy_command_number = 0x10,
			has_client_tick = 0x20,
			has_forwardmove = 0x40,
			has_leftmove = 0x80,
			has_upmove = 0x100,
			has_impulse = 0x200,
			has_weaponselect = 0x400,
			has_random_seed = 0x800,
			has_mousedx = 0x1000,
			has_mousedy = 0x2000,
			has_prediction_offset_ticks_x256 = 0x4000,
			has_consumed_server_angle_changes = 0x8000,
			has_cmd_flags = 0x10000,
			has_pawn_entity_handle = 0x20000
		};

		char pad1[0x10];
		std::uint32_t has_bits;
		std::int32_t cached_size;
		protobuf_repeated_ptr_field_t subtick_moves;
		void* move_crc;
		cin_button_state_pb_t* buttons;
		QAngle* view_angles;
		void* execution_notes;
		std::int32_t legacy_command_number;
		std::int32_t client_tick;
		float forward_move;
		float side_move;
		float up_move;
		std::int32_t impulse;
		std::int32_t weaponselect;
		std::int32_t random_seed;
		std::int32_t mousedx;
		std::int32_t mousedy;
		std::uint32_t prediction_offset_ticks_x256;
		std::uint32_t consumed_server_angle_changes;
		std::int32_t cmd_flags;
		std::uint32_t pawn_entity_handle;
	};
	class csgo_user_cmd_pb_t {
	public:
		char pad1[0x10];
		std::uint32_t has_bits;
		std::int32_t cached_size;
		protobuf_repeated_ptr_field_t input_history;
		cbase_user_cmd_pb_t* base;
		bool left_hand_desired;
		bool is_predicting_body_shot_fx;
		bool is_predicting_head_shot_fx;
		bool is_predicting_kill_ragdolls;
		std::int32_t attack1_start_history_index;
		std::int32_t attack2_start_history_index;
	};

	class user_cmd_t {
	public:
		bool set_input_history(const vec3_t& angles, controller_t* controller, player_t* player, bool preserve_timing) {
			if (!controller || !player) return false;
			if (!std::isfinite(angles.x) || !std::isfinite(angles.y) || !std::isfinite(angles.z)) return false;

			constexpr float interval = 1.f / 64.f;
			const float simulation_time = preserve_timing ? 0.f : player->simulation_time();
			if (!std::isfinite(simulation_time) || simulation_time < 0.f) return false;
			const double render_tick = std::floor(0.5 + static_cast<double>(simulation_time) / interval) + 1.0;
			const auto tick_base = controller->tick_base();
			if (render_tick > (std::numeric_limits<std::int32_t>::max)() || tick_base > static_cast<std::uint32_t>((std::numeric_limits<std::int32_t>::max)())) return false;

			auto& history = cmd.input_history;
			auto rep = history.rep;
			if (!rep) return false;
			const int count = (std::min)({ history.current_size, history.capacity, rep->allocated_size });
			if (count <= 0) return false;

			const float fraction = std::nextafterf(1.f, 0.f);
			bool changed = false;
			for (int i = 0; i < count; i++) {
				auto entry = static_cast<csgo_input_history_entry_pb_t*>(rep->elements[i]);
				if (!entry || !entry->view_angles) continue;
				entry->view_angles->angles = angles;
				entry->has_bits |= csgo_input_history_entry_pb_t::has_viewangles;
				changed = true;
				if (preserve_timing) continue;
				entry->player_tick_count = static_cast<std::int32_t>(tick_base);
				entry->player_tick_fraction = fraction;
				entry->render_tick_count = static_cast<std::int32_t>(render_tick);
				entry->render_tick_fraction = 0.f;
				entry->has_bits |= csgo_input_history_entry_pb_t::has_viewangles | csgo_input_history_entry_pb_t::has_player_tick_count | csgo_input_history_entry_pb_t::has_player_tick_fraction | csgo_input_history_entry_pb_t::has_render_tick_count	| csgo_input_history_entry_pb_t::has_render_tick_fraction;
				changed = true;
			}
			return changed;
		}

		void* vtable;
		std::int64_t command_number_value;
		csgo_user_cmd_pb_t cmd;
		void* button_state_vtable;
		std::uint64_t buttons;
		std::uint64_t buttons2;
		std::uint64_t buttons3;
		char pad1[0x1C];
		std::int32_t subtick_state;

		cbase_user_cmd_pb_t* get_base() {
			cbase_user_cmd_pb_t* base = cmd.base;
			return base;
		}

		bool valid() const { return cmd.base != nullptr; }

		cin_button_state_pb_t* get_proto_buttons() {
			cbase_user_cmd_pb_t* base = get_base();
			if (!base) return nullptr;
			cin_button_state_pb_t* buttons = base->buttons;

			return buttons;
		}

		QAngle* get_view_angles() {
			cbase_user_cmd_pb_t* base = get_base();

			if (!base) return nullptr;
			return base->view_angles;
		}

		bool is_button_down(std::uint64_t buttons) {
			return (this->buttons & buttons) != 0;
		}

		std::int32_t command_number() {
			return static_cast<std::int32_t>(command_number_value);
		}

		bool has_been_predicted() {
			return *reinterpret_cast<const bool*>(reinterpret_cast<const std::uint8_t*>(this) + 0x88);
		}

		void set_has_been_predicted(bool value) {
			*reinterpret_cast<bool*>(reinterpret_cast<std::uint8_t*>(this) + 0x88) = value;
		}

		bool update_move_crc() {
			cbase_user_cmd_pb_t* base = get_base();

			if (!base) return false;

			using function_t = void(__fastcall*)(void*, void*, void*);
			static function_t serialize_move_crc = reinterpret_cast<function_t>(mem::find_pattern("client.dll", patts::SerializeMoveCrc));

			if (!serialize_move_crc) return false;

			const cin_button_state_pb_t* buttons = base->buttons;
			const QAngle* view = base->view_angles;

			const std::uint64_t button_states[3] = {
				buttons ? buttons->buttonstate1 : 0ull,
				buttons ? buttons->buttons2 : 0ull,
				buttons ? buttons->buttons3 : 0ull
			};
			const float angles[3] = { view ? view->angles.x : 0.f, view ? view->angles.y : 0.f, view ? view->angles.z : 0.f };

			std::uint8_t buffer[64] = { };
			std::uint8_t* cursor = buffer;
			std::uint8_t button_size = 0;

			for (const std::uint64_t state : button_states) if (state != 0) button_size += 9;
			if (button_size != 0) {
				*cursor++ = 0x1a;
				*cursor++ = button_size;

				for (std::size_t i = 0; i < 3; ++i)
				{
					if (button_states[i] == 0) continue;

					*cursor++ = static_cast<std::uint8_t>(0x09 + i * 0x08);
					std::memcpy(cursor, &button_states[i], sizeof(std::uint64_t));
					cursor += sizeof(std::uint64_t);
				}
			}

			std::uint8_t angle_size = 0;

			for (const float angle : angles) if (angle != 0.f) angle_size += 5;
			if (angle_size != 0) {
				*cursor++ = 0x22;
				*cursor++ = angle_size;

				for (std::size_t i = 0; i < 3; ++i)
				{
					if (angles[i] == 0.f) continue;

					*cursor++ = static_cast<std::uint8_t>(0x0d + i * 0x08);
					std::memcpy(cursor, &angles[i], sizeof(float));
					cursor += sizeof(float);
				}
			}

			const std::uintptr_t arena_address = reinterpret_cast<std::uintptr_t>(base) + 0x8;
			utl_string_t payload = { reinterpret_cast<char*>(buffer), 0, 0, static_cast<std::int32_t>(cursor - buffer) };

			const std::uintptr_t arena_bits = *reinterpret_cast<std::uintptr_t*>(arena_address);
			std::uintptr_t arena = arena_bits & ~static_cast<std::uintptr_t>(0x3);

			if ((arena_bits & 1) != 0) 	arena = *reinterpret_cast<std::uintptr_t*>(arena);
			serialize_move_crc(&base->move_crc, &payload, reinterpret_cast<void*>(arena));
			base->has_bits |= cbase_user_cmd_pb_t::has_move_crc;

			return true;
		}

		void sync_buttons() {
			cin_button_state_pb_t* buttons = get_proto_buttons();

			if (!buttons) return;

			cbase_user_cmd_pb_t* base = get_base();
			if (!base) return;

			base->has_bits |= cbase_user_cmd_pb_t::has_buttons;
			buttons->has_bits |= 0x7u;
			buttons->buttonstate1 = this->buttons;
			buttons->buttons2 = buttons2;
			buttons->buttons3 = buttons3;
		}

		void set_buttons(std::uint64_t buttons, bool state)
		{
			if (state)
			{
				this->buttons |= buttons;
				buttons2 |= buttons;
			}
			else
			{
				this->buttons &= ~buttons;
				buttons2 &= ~buttons;
				buttons3 &= ~buttons;
			}

			sync_buttons();
		}

		void set_button(buttons_t button, bool state) {
			set_buttons(static_cast<std::uint64_t>(button), state);
		}

		void set_move(float forward, float left) {
			cbase_user_cmd_pb_t* base = get_base();

			if (!base) return;
			base->has_bits |= cbase_user_cmd_pb_t::has_forwardmove | cbase_user_cmd_pb_t::has_leftmove;
			base->forward_move = forward;
			base->side_move = left;
		}

		void set_view_angles(vec3_t angles)
		{
			if (!std::isfinite(angles.x) || !std::isfinite(angles.y) || !std::isfinite(angles.z)) return;
			QAngle* view = get_view_angles();

			if (!view) return;
			view->angles.x = angles.x;
			view->angles.y = angles.y;
		}

		void clear_subticks()
		{
			cbase_user_cmd_pb_t* base = get_base();
			if (!base) return;
			base->subtick_moves.current_size = 0;
		}

		subtick_move_step_t* get_subtick(std::int32_t index) {
			cbase_user_cmd_pb_t* base = get_base();

			if (!base) return nullptr;
			if (index < 0 || index >= base->subtick_moves.current_size) return nullptr;

			protobuf_repeated_ptr_field_t::rep_t* rep = base->subtick_moves.rep;
			if (!rep) return nullptr;
			if (index >= rep->allocated_size || index >= base->subtick_moves.capacity) return nullptr;

			subtick_move_step_t* step = reinterpret_cast<subtick_move_step_t*>(rep->elements[index]);
			return step;
		}

		std::int32_t subtick_count() {
			cbase_user_cmd_pb_t* base = get_base();
			return base ? base->subtick_moves.current_size : 0;
		}

		bool has_movement_subticks() {
			const std::int32_t count = subtick_count();
			for (std::int32_t i = 0; i < count; i++)
			{
				subtick_move_step_t* step = get_subtick(i);
				if (step && step->has_movement()) return true;
			}

			return false;
		}

		subtick_move_step_t* acquire_subtick() {
			cbase_user_cmd_pb_t* base = get_base();
			if (!base) return nullptr;

			protobuf_repeated_ptr_field_t& moves = base->subtick_moves;
			if (moves.current_size < 0 || moves.current_size >= 30) return nullptr;
			if (moves.rep && moves.current_size < moves.rep->allocated_size)
			{
				auto step = reinterpret_cast<subtick_move_step_t*>(moves.rep->elements[moves.current_size]);
				if (step)
				{
					moves.current_size++;
					if (moves.capacity < moves.current_size) moves.capacity = moves.current_size;
					return step;
				}
			}

			using allocate_t = subtick_move_step_t * (__fastcall*)(void*);
			using append_t = subtick_move_step_t * (__fastcall*)(protobuf_repeated_ptr_field_t*, subtick_move_step_t*);
			static allocate_t allocate = []() -> allocate_t {
					auto match = mem::find_pattern("client.dll", patts::CreateSubtickMoveStep);
					return match ? reinterpret_cast<allocate_t>(mem::resolve_absolute_rip_address(match + 16, 1, 5)) : nullptr;
				}();
			static append_t append = []() -> append_t {
					auto match = mem::find_pattern("client.dll", patts::ProtoBuffAddPtrEtc);
					return match ? reinterpret_cast<append_t>(mem::resolve_absolute_rip_address(match, 1, 5)) : nullptr;
				}();
			if (!allocate || !append) 	return nullptr;
			auto step = allocate(moves.arena);
			if (!step) return nullptr;
			return append(&moves, step);
		}

		void add_subtick(buttons_t button, bool pressed, float when) {
			subtick_move_step_t* step = acquire_subtick();

			if (!step) return;
			step->set_button(static_cast<std::uint64_t>(button));
			step->set_pressed(pressed);
			step->set_when(when);
		}

		auto add_movement_subtick(float when, float forward_delta, float left_delta, float yaw_delta = 0.f, float pitch_delta = 0.f) {
			subtick_move_step_t* step = acquire_subtick();

			if (!step) return;
			step->set_button(0);
			step->set_pressed(false);
			step->set_when(when);
			step->set_analog_forward_delta(forward_delta);
			step->set_analog_left_delta(left_delta);

			if (yaw_delta != 0.f) step->set_yaw_delta(yaw_delta);
			if (pitch_delta != 0.f) step->set_pitch_delta(pitch_delta);
		}

		bool is_subtick_overwrite() const {
			return subtick_state == 2;
		}
	};
}
