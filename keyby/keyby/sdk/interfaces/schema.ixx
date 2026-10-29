module;
#include "../../includes.hpp"
#include <unordered_set>
export module schema;
import memory;
import patts;

export typedef struct _schema_class_field_data_t
{
	const char* field_name;
	void* field_type;
	std::int32_t field_offset;
	char pad1[0xC];
} schema_class_field_data_t;

export class schema_class_binding_t {
public:
	char pad1[0x8];
	const char* binding_name;
	const char* dll_name;
	char pad2[0x8];
	std::int32_t size_of;
	std::uint16_t data_array_size;
	char pad3[0x2];
	char pad4[0x8];
	schema_class_field_data_t* data_array;
	void* base_class;
};

export class schema_block_t {
public:
	void* unkn0;
	schema_block_t* next;
	schema_class_binding_t* binding;
};

export class schema_block_container_t {
public:
	void* unkn[2];
	schema_block_t* first_block;
};

export class schema_class_list_t {
public:
	std::array<schema_block_container_t, 256> block_containers;
	std::int32_t GetNumSchema() {
		return *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uintptr_t>(this) - 0x74);
	}
};

export class schema_system_type_scope {
public:
	schema_class_list_t* GetClassContainer() {
		return reinterpret_cast<schema_class_list_t*>(reinterpret_cast<std::uintptr_t>(this) + 0x5C0);
	}
};

export class i_schema_system {
public:
	schema_system_type_scope* GlobalTypeScope() {
		using function_t = schema_system_type_scope *(__thiscall*)(void*);
		return (*reinterpret_cast<function_t**>(this))[11](this);
	}
};

export namespace schema_system
{
	typedef struct _schema_offset_t
	{
		std::string class_name;
		std::string property_name;
		std::uint32_t offset;
	} schema_offset_t;

	void bind(i_schema_system* system);
	bool initialize();
	bool has_schema(std::string_view class_name, std::string_view property_name);
	std::uint32_t get_schema(std::string_view class_name, std::string_view property_name);
    template <std::size_t N> struct field_name {
        char value[N];
        constexpr field_name(const char (&text)[N]) {
            for (std::size_t i = 0; i < N; ++i) value[i] = text[i];
        }
    };
    template <class T, field_name Class, field_name Property>
    T& field(void* instance) {
        static const auto offset = get_schema(Class.value, Property.value);
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(instance) + offset);
    }
}

namespace {
    i_schema_system* schema_instance = nullptr;
    std::unordered_map<std::string, std::uint32_t> offsets;
    template <class T> T read(const void* address) {
        T value{};
        if (!ReadProcessMemory(GetCurrentProcess(), address, &value, sizeof(value), nullptr))
            throw std::runtime_error("Schema memory is unreadable");
        return value;
    }
    std::string read_name(const char* address) {
        std::string name;
        for (std::size_t i = 0; i < 256; ++i) {
            const char ch = read<char>(address + i);
            if (!ch) return name;
            name += ch;
        }
        throw std::runtime_error("Schema name is too long");
    }
    std::string key(std::string_view type, std::string_view field) {
        return std::string(type) + "::" + std::string(field);
    }
}

bool schema_system::initialize() {
    if (!schema_instance) return false;
    try {
        std::vector<schema_system_type_scope*> scopes;
        scopes.push_back(schema_instance->GlobalTypeScope());
        auto* address = mem::resolve_absolute_rip_address(mem::find_pattern("schemasystem.dll", patts::AllTypeScopes), 3, 7);
        auto** array = read<schema_system_type_scope**>(address);
        const auto count = read<std::uint16_t>(address - 8);
        if (count > 1024) return false;
        for (std::uint16_t i = 0; array && i < count; ++i) scopes.push_back(read<schema_system_type_scope*>(array + i));
        std::unordered_map<std::string, std::uint32_t> found;
        std::unordered_set<schema_block_t*> visited;
        for (auto* scope : scopes) {
            if (!scope) continue;
            auto* list = scope->GetClassContainer();
            const auto containers = read<std::array<schema_block_container_t, 256>>(list);
            for (const auto& container : containers) {
                auto* block = container.first_block;
                while (block && visited.insert(block).second) {
                    if (visited.size() > 65536) return false;
                    const auto node = read<schema_block_t>(block);
                    block = node.next;
                    if (!node.binding) continue;
                    const auto binding = read<schema_class_binding_t>(node.binding);
                    if (!binding.binding_name || !binding.data_array) continue;
                    if (binding.data_array_size > 4096) return false;
                    const auto class_name = read_name(binding.binding_name);
                    for (std::uint16_t i = 0; i < binding.data_array_size; ++i) {
                        const auto field = read<schema_class_field_data_t>(binding.data_array + i);
                        if (field.field_name && field.field_offset >= 0)
                            found.insert_or_assign(key(class_name, read_name(field.field_name)), field.field_offset);
                    }
                }
            }
        }
        offsets = std::move(found);
        return !offsets.empty();
    } catch (const std::exception&) { return false; }
}

bool schema_system::has_schema(std::string_view class_name, std::string_view property_name) {
    if (offsets.empty() && !initialize()) return false;
    return offsets.contains(key(class_name, property_name));
}

std::uint32_t schema_system::get_schema(std::string_view class_name, std::string_view property_name) {
    if (offsets.empty() && !initialize()) throw std::runtime_error("Schema initialization failed");
    const auto field = offsets.find(key(class_name, property_name));
    if (field == offsets.end())
        throw std::runtime_error(std::format("Schema field not found: {}::{}", class_name, property_name));
    return field->second;
}
void schema_system::bind(i_schema_system* system) {
    schema_instance = system;
    offsets.clear();
}
