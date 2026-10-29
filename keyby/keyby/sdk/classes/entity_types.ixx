module;
#include "../../includes.hpp"
export module entity_types;
export import math;
import schema;

export {
    using entity_handle_t = std::uint32_t;
    inline constexpr entity_handle_t invalid_entity_handle = 0xFFFFFFFF;

    template <class T> struct utl_vector_t {
        std::int32_t size;
        std::int32_t debug_info;
        T* memory;
        std::int32_t capacity;
        std::int32_t grow_size;
        bool is_valid_index(std::int32_t index) const { return index >= 0 && index < size; }
    };
    static_assert(sizeof(utl_vector_t<entity_handle_t>) == 0x18);

    struct alignas(16) bone_data_t {
        vec3_t position;
        float scale;
        vec3_t rotation;
    };
    static_assert(sizeof(bone_data_t) == 0x20);

    struct model_state_t {
        std::byte reserved[0x80];
        bone_data_t* bones;
        std::uint32_t reserved_count;
        std::uint32_t bone_count;
    };
    static_assert(offsetof(model_state_t, bones) == 0x80);
    static_assert(offsetof(model_state_t, bone_count) == 0x8C);

    class skeleton_instance_t {
    public:
        model_state_t& model_state() { return schema_system::field<model_state_t, "CSkeletonInstance", "m_modelState">(this); }
    };

    class game_scene_node_t {
    public:
        vec3_t& abs_origin() { return schema_system::field<vec3_t, "CGameSceneNode", "m_vecAbsOrigin">(this); }
        skeleton_instance_t* skeleton_instance() { return reinterpret_cast<skeleton_instance_t*>(this); }
    };

    class entity_t {
    public:
        game_scene_node_t*& game_scene_node() { return schema_system::field<game_scene_node_t*, "C_BaseEntity", "m_pGameSceneNode">(this); }
        std::uint8_t& team() { return schema_system::field<std::uint8_t, "C_BaseEntity", "m_iTeamNum">(this); }
        vec3_t abs_origin() {
            auto* scene = game_scene_node();
            return scene ? scene->abs_origin() : vec3_t{};
        }
    };

    class model_entity_t : public entity_t {
    public:
        vec3_t& view_offset() { return schema_system::field<vec3_t, "C_BaseModelEntity", "m_vecViewOffset">(this); }
    };

    class entity_spotted_state_t {
    public:
        bool& spotted() { return schema_system::field<bool, "EntitySpottedState_t", "m_bSpotted">(this); }
    };

    class econ_item_view_t {
    public:
        std::uint16_t& item_definition_index() { return schema_system::field<std::uint16_t, "C_EconItemView", "m_iItemDefinitionIndex">(this); }
        std::uint64_t& item_id() { return schema_system::field<std::uint64_t, "C_EconItemView", "m_iItemID">(this); }
    };

}
