module;
export module memory;
#include "../../includes.hpp"

namespace sdk {
	template <std::size_t size>
	class padding {
	private:
		auto m_pad[size];
	};
	template <class vfunc = void*, std::size_t Index>
	vfunc getvfunc(void* fmt) {
		return (*static_cast<vfunc**>(fmt))[Index];
	}
	auto* pattern(const std::string_view& module_name, const std::string_view& pat);
	auto* resolve_absolute_rip_address(std::uint8_t* inst, std::size_t offset_to_disp, std::size_t inst_disp);
}