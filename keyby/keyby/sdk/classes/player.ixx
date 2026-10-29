module;
#include "../../includes.hpp"

export module players_class;
import memory;
import math;
import schema;
import patts;
import types;
export import entity_types;

export {
	class player_weapon_services_t {
	public:
		entity_handle_t& active_weapon() { return schema_system::field<entity_handle_t, "CPlayer_WeaponServices", "m_hActiveWeapon">(this); }
		utl_vector_t<entity_handle_t>& weapons() { return schema_system::field<utl_vector_t<entity_handle_t>, "CPlayer_WeaponServices", "m_hMyWeapons">(this); }
	};

	class cs_player_weapon_services_t : public player_weapon_services_t
	{
	public:
	};

	class player_item_services_t
	{
	public:
	};

	class cs_player_item_services_t : public player_item_services_t {
	public:
		bool& has_defuser() { return schema_system::field<bool, "CCSPlayer_ItemServices", "m_bHasDefuser">(this); }
		bool& has_helmet() { return schema_system::field<bool, "CCSPlayer_ItemServices", "m_bHasHelmet">(this); }
	};

	class player_observer_services_t {
	public:
		std::uint8_t& observer_mode() { return schema_system::field<std::uint8_t, "CPlayer_ObserverServices", "m_iObserverMode">(this); }
		entity_handle_t& observer_target() { return schema_system::field<entity_handle_t, "CPlayer_ObserverServices", "m_hObserverTarget">(this); }
	};

	class player_camera_services_t {
	public:
		vec3_t& view_punch_angle() { return schema_system::field<vec3_t, "CPlayer_CameraServices", "m_vecCsViewPunchAngle">(this); }
		entity_handle_t& active_post_processing_volume() { return schema_system::field<entity_handle_t, "CPlayer_CameraServices", "m_hActivePostProcessingVolume">(this); }
	};

	class cs_player_base_camera_services_t : public player_camera_services_t
	{
	public:
		std::uint32_t& fov() { return schema_system::field<std::uint32_t, "CCSPlayerBase_CameraServices", "m_iFOV">(this); }
	};

	class CCSPlayerModernJump {
	public:
		float& m_flLastLandedFrac() { return schema_system::field<float, "CCSPlayerModernJump", "m_flLastLandedFrac">(this); }
	};

	class player_movement_services_t {
	public:
		std::uint64_t& queued_button_down_mask() { return schema_system::field<std::uint64_t, "CPlayer_MovementServices", "m_nQueuedButtonDownMask">(this); }
		std::uint64_t& queued_button_change_mask() { return schema_system::field<std::uint64_t, "CPlayer_MovementServices", "m_nQueuedButtonChangeMask">(this); }

		std::uint64_t& buttondoublepressed() { return schema_system::field<std::uint64_t, "CPlayer_MovementServices", "m_nButtonDoublePressed">(this); }
		std::uint64_t& toggle_button_down_mask() { return schema_system::field<std::uint64_t, "CPlayer_MovementServices", "m_nToggleButtonDownMask">(this); }

		std::uint32_t& last_command_number_processed() { return schema_system::field<std::uint32_t, "CPlayer_MovementServices", "m_nLastCommandNumberProcessed">(this); }
		float& max_speed() { return schema_system::field<float, "CPlayer_MovementServices", "m_flMaxSpeed">(this); }

		float& surface_friction() { return schema_system::field<float, "CPlayer_MovementServices_Humanoid", "m_flSurfaceFriction">(this); }
		float& step_sound_time() { return schema_system::field<float, "CPlayer_MovementServices_Humanoid", "m_flStepSoundTime">(this); }

		std::uint64_t& button_down_mask_prev() { return schema_system::field<std::uint64_t, "CCSPlayer_MovementServices", "m_nButtonDownMaskPrev">(this); }
		std::uint64_t& cmd_forward_mv() { return schema_system::field<std::uint64_t, "CCSPlayer_MovementServices", "m_flCmdForwardMove">(this); }
		CCSPlayerModernJump& modern_jump() { return schema_system::field<CCSPlayerModernJump, "CCSPlayer_MovementServices", "m_ModernJump">(this); }

		std::uint64_t trace_mask() const {
			return *reinterpret_cast<const std::uint64_t*>(reinterpret_cast<const std::uint8_t*>(this) + 0xd48);
		}

		std::uint32_t trace_state() const {
			return *reinterpret_cast<const std::uint32_t*>(reinterpret_cast<const std::uint8_t*>(this) + 0x3f8);
		}

		template <typename command_t>
		void set_prediction_command(command_t* cmd)
		{
			using function_t = void (*)(player_movement_services_t*, command_t*);
			mem::getvfunc<function_t, 46>(this)(this, cmd);
		}

		template <typename command_t>
		void run_command(command_t* cmd)
		{
			using function_t = void (*)(player_movement_services_t*, command_t*);
			mem::getvfunc<function_t, 32>(this)(this, cmd);
		}

		void reset_prediction_command()
		{
			using function_t = void (*)(player_movement_services_t*);
			mem::getvfunc<function_t, 47>(this)(this);
		}
	};

	class base_player_pawn_t : public model_entity_t {
	public:
		player_movement_services_t*& movement_services() { return schema_system::field<player_movement_services_t*, "C_BasePlayerPawn", "m_pMovementServices">(this); }
		cs_player_weapon_services_t*& weapon_services() { return schema_system::field<cs_player_weapon_services_t*, "C_BasePlayerPawn", "m_pWeaponServices">(this); }
		cs_player_item_services_t*& item_services() { return schema_system::field<cs_player_item_services_t*, "C_BasePlayerPawn", "m_pItemServices">(this); }
		player_observer_services_t*& observer_services() { return schema_system::field<player_observer_services_t*, "C_BasePlayerPawn", "m_pObserverServices">(this); }
		cs_player_base_camera_services_t*& camera_services() { return schema_system::field<cs_player_base_camera_services_t*, "C_BasePlayerPawn", "m_pCameraServices">(this); }
		vec3_t& old_origin() { return schema_system::field<vec3_t, "C_BasePlayerPawn", "m_vOldOrigin">(this); }
		entity_handle_t& controller_handle() { return schema_system::field<entity_handle_t, "C_BasePlayerPawn", "m_hController">(this); }
		float& fov_sensitivity_adjust() { return schema_system::field<float, "C_BasePlayerPawn", "m_flFOVSensitivityAdjust">(this); }

		player_movement_services_t* movement() {
			return movement_services();
		}

		cs_player_weapon_services_t* weapons() {
			return weapon_services();
		}

		cs_player_item_services_t* items() {
			return item_services();
		}

		cs_player_base_camera_services_t* camera() {
			return camera_services();
		}
	};

	class cs_player_pawn_base_t : public base_player_pawn_t {
	public:
		float& FlashTime() { return schema_system::field<float, "C_CSPlayerPawnBase", "m_flFlashBangTime">(this); }
		float& FlashMaxAlpha() { return schema_system::field<float, "C_CSPlayerPawnBase", "m_flFlashMaxAlpha">(this); }
		float& FlashDuration() { return schema_system::field<float, "C_CSPlayerPawnBase", "m_flFlashDuration">(this); }
		float& LastSpawnTimeIndex() { return schema_system::field<float, "C_CSPlayerPawnBase", "m_flLastSpawnTimeIndex">(this); }
	};

	class player_t : public cs_player_pawn_base_t {
	public:
		void*& AimPunchService() { return schema_system::field<void*, "C_CSPlayerPawn", "m_pAimPunchServices">(this); }
		std::int32_t& ArmorVal() { return schema_system::field<std::int32_t, "C_CSPlayerPawn", "m_ArmorValue">(this); }
		bool& GunImmunity() { return schema_system::field<bool, "C_CSPlayerPawn", "m_bGunGameImmunity">(this); }
		vec3_t& EyeAngles() { return schema_system::field<vec3_t, "C_CSPlayerPawn", "m_angEyeAngles">(this); }
		std::int32_t& ShotsFired() { return schema_system::field<std::int32_t, "C_CSPlayerPawn", "m_iShotsFired">(this); }
		bool& IsDefusing() { return schema_system::field<bool, "C_CSPlayerPawn", "m_bIsDefusing">(this); }
		bool& IsScoped() { return schema_system::field<bool, "C_CSPlayerPawn", "m_bIsScoped">(this); }
		bool& WaitForNoAttack() { return schema_system::field<bool, "C_CSPlayerPawn", "m_bWaitForNoAttack">(this); }
		auto& health() { return schema_system::field<std::int32_t, "C_BaseEntity", "m_iHealth">(this); }
		entity_spotted_state_t& Spotted() { return schema_system::field<entity_spotted_state_t, "C_CSPlayerPawn", "m_entitySpottedState">(this); }
		econ_item_view_t& EconGloves() { return schema_system::field<econ_item_view_t, "C_CSPlayerPawn", "m_EconGloves">(this); }
		entity_handle_t& HudModelArms() { return schema_system::field<entity_handle_t, "C_CSPlayerPawn", "m_hHudModelArms">(this); }
		float& DetectedByEnemySensorTime() { return schema_system::field<float, "C_CSPlayerPawn", "m_flDetectedByEnemySensorTime">(this); }
		vec3_t& Velocity() { return schema_system::field<vec3_t, "C_BaseEntity", "m_vecVelocity">(this); }
		vec3_t& AbsVelocity() { return schema_system::field<vec3_t, "C_BaseEntity", "m_vecAbsVelocity">(this); }

		vec3_t GetEyePosition() { return abs_origin() + view_offset(); }
		vec3_t GetEyePos() { return GetEyePosition(); }
		vec3_t GetAimPunch() {
			if (!AimPunchService()) return {};
			using fn_t = void(__fastcall*)(void*, vec3_t*, std::int32_t);
			static auto fn = reinterpret_cast<fn_t>(mem::find_pattern("client.dll", patts::GetAimPunch));
			vec3_t punch{};
			if (fn) fn(AimPunchService(), &punch, 0);
			return punch;
		}

		bool HasDefuser() { auto* s = items(); return s && s->has_defuser(); }
		bool HasHelmet() { auto* s = items(); return s && s->has_helmet(); }
		bool HasGunImmunity() {
			static const std::uint32_t offset = schema_system::get_schema("C_CSPlayerPawn", "m_bGunGameImmunity");
			return offset != 0 && this->GunImmunity();
		}

		bool HasArmor(std::int32_t hitgroup) { return hitgroup == 1 ? HasHelmet() : ArmorVal() > 0; }

		std::int32_t  GetBoneIndex(const auto* bone_name) {
			using function_t = std::int32_t(__fastcall*)(player_t*, const char*);
			static function_t fn = reinterpret_cast<function_t>(mem::find_pattern("client.dll", patts::GetBoneId));
			if (!fn || !bone_name) return -1;
			const auto index = fn(this, bone_name);
			return index >= 0 && index <= 127 ? index : -1;
		}
		std::int32_t GetBoneIndex(boneindex bone) { return static_cast<std::int32_t>(bone); }

		bool IsAlive() { return health() > 0; }
		auto UpdateBones() {
			game_scene_node_t* scene_node = this->game_scene_node();

			if (!scene_node) return;
			using function_t = void(__fastcall*)(skeleton_instance_t*, std::uint32_t);
			static function_t update_bones_fn = reinterpret_cast<function_t>(mem::find_pattern("client.dll", patts::CalcBones));

			if (update_bones_fn) update_bones_fn(scene_node->skeleton_instance(), 0xFFFFF);
		}

		bone_data_t* get_bone_cache(std::int32_t* out_count) {
			if (out_count) *out_count = 0;
			game_scene_node_t* scene_node = this->game_scene_node();

			if (!scene_node) return nullptr;

			model_state_t& model_state = scene_node->skeleton_instance()->model_state();

			bone_data_t* bones = model_state.bones;
			const std::int32_t count = static_cast<std::int32_t>(model_state.bone_count);

			if (!mem::is_readable(bones, sizeof(bone_data_t) * count)) return nullptr;
			if (out_count) *out_count = count;
			return bones;
		}

		vec3_t GetBonePosition(std::int32_t bone_index) {
			if (bone_index < 0 || bone_index > 127)	return vec3_t();
			this->UpdateBones();
			game_scene_node_t* scene_node = this->game_scene_node();

			if (!scene_node)	return vec3_t();
			bone_data_t* bones = scene_node->skeleton_instance()->model_state().bones;

			if (!mem::is_readable(bones, sizeof(bone_data_t) * (bone_index + 1)))	return vec3_t();
			return bones[bone_index].position;
		}

		vec3_t GetBonePosition(const auto* bone_name) { return GetBonePosition(GetBoneIndex(bone_name)); }
	};

	class base_player_controller_t : public entity_t {
	public:
		std::uint32_t& TickBase() { return schema_system::field<std::uint32_t, "CBasePlayerController", "m_nTickBase">(this); }
		entity_handle_t& PawnHandle() { return schema_system::field<entity_handle_t, "CBasePlayerController", "m_hPawn">(this); }
		std::uint64_t& SteamId() { return schema_system::field<std::uint64_t, "CBasePlayerController", "m_steamID">(this); }
		bool& IsLocalPlrController() { return schema_system::field<bool, "CBasePlayerController", "m_bIsLocalPlayerController">(this); }
	};

	class in_game_money_services_t {
	public:
		std::int32_t& Account() { return schema_system::field<std::int32_t, "CCSPlayerController_InGameMoneyServices", "m_iAccount">(this); }
	};

	class inventory_services_t {
	public:
		std::uint16_t& MusicId() { return schema_system::field<std::uint16_t, "CCSPlayerController_InventoryServices", "m_unMusicID">(this); }
	};

	class controller_t : public base_player_controller_t {
	public:
		in_game_money_services_t*& MoneyServices() { return schema_system::field<in_game_money_services_t*, "CCSPlayerController", "m_pInGameMoneyServices">(this); }
		inventory_services_t*& InventoryServices() { return schema_system::field<inventory_services_t*, "CCSPlayerController", "m_pInventoryServices">(this); }
		const char*& Name() { return schema_system::field<const char*, "CCSPlayerController", "m_sSanitizedPlayerName">(this); }
		bool& IsAlive() { return schema_system::field<bool, "CCSPlayerController", "m_bPawnIsAlive">(this); }
		std::uint32_t& Ping() { return schema_system::field<std::uint32_t, "CCSPlayerController", "m_iPing">(this); }
		entity_handle_t& Observed() { return schema_system::field<entity_handle_t, "CCSPlayerController", "m_hObserverPawn">(this); }
	};
}
