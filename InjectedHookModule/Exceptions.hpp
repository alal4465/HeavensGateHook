#pragma once
#include <stdexcept>
#include <string>
#include <Windows.h>

#define DEFINE_WIN32_EXCEPTION(cls, msg)  \
class cls : Win32Exception {              \
public:                                   \
	cls() : Win32Exception(msg) {}        \
};

namespace wow64_hook {
	namespace winapi {
		class Win32Exception : std::runtime_error {
		public:
			explicit Win32Exception(const std::string& msg="") : 
				std::runtime_error(msg + " | Last Error: " + std::to_string(::GetLastError())) {}
		};
		
		DEFINE_WIN32_EXCEPTION(GetModuleHandleException, "GetModuleHandle failed");
		DEFINE_WIN32_EXCEPTION(GetProcAddressException, "GetProcAddress failed");
		DEFINE_WIN32_EXCEPTION(VirtualProtectException, "VirtualProtect failed");
	}
}
