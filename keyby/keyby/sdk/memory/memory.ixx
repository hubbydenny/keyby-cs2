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
	std::uint8_t* pattern(const std::string_view& module_name, const std::string_view& pat);
	std::uint8_t* resolve_absolute_rip_address(std::uint8_t* inst, std::size_t offset_to_disp, std::size_t inst_disp);
}
