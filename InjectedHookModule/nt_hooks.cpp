#include "nt_hooks.hpp"

namespace wow64_hook {
	std::array<HookedWow64Syscall, 1> g_hooked_nt_routines_arr = { {0x60034, HookedNtDelayExecution} };

	void __cdecl HookedNtDelayExecution(Wow64SyscallState state, BOOLEAN Alertable, PLARGE_INTEGER DelayInterval) {
		std::cout << "Hello from hooked syscall :)\n";
		std::cout << "Alterable: " << static_cast<int>(Alertable) << '\n';
		std::cout << "DelayInterval: " << DelayInterval->QuadPart << '\n';
	}
}
