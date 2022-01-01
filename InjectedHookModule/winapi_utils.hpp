#pragma once
#include <Windows.h>
#include <cstdint>
#include "Exceptions.hpp"

namespace wow64_hook {
	namespace winapi {
		/// Memory protection constants wrapped in an enum class
		enum class MemoryProtection: unsigned long {
			Exec = PAGE_EXECUTE,
			ExecRead = PAGE_EXECUTE_READ,
			ExecReadWrite = PAGE_EXECUTE_READWRITE,
			ReadOnly = PAGE_READONLY,
			ReadWrite = PAGE_READWRITE,
			NoAccess = PAGE_NOACCESS
		};

		/// a VirtualProtect wrapper to change memory protections
		void mem_protect(void* addr, std::uint32_t size, MemoryProtection prot);
	}
}
