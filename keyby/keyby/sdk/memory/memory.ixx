module;
#include "../../includes.hpp"

export module memory;

namespace mem {
	export template <std::size_t size>
	
	class padding {
	private:
		std::uint8_t m_pad[size];
	};

	export template <class vfunc = void*, std::size_t Index>
	vfunc getvfunc(void* fmt) {
		return (*static_cast<vfunc**>(fmt))[Index];
	}
	export std::uint8_t* find_pattern(const char* module_name, const char* pattern);
	export void* read_pointer(const void* address);
	export bool is_readable(const void* address, std::size_t size);
	export std::uint8_t* resolve_absolute_rip_address(std::uint8_t* inst, std::size_t offset_to_disp, std::size_t inst_disp);
}
