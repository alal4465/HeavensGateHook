#include "winapi_utils.hpp"

namespace wow64_hook {
	namespace winapi {
		void mem_protect(void* addr, std::uint32_t size, MemoryProtection prot) {
			unsigned long old_prot;
			if (!::VirtualProtect(addr, size, 
				static_cast<unsigned long>(prot), &old_prot)) {
				throw VirtualProtectException();
			}
		}
	}
}