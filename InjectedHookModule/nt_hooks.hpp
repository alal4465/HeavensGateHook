#pragma once
#include <array>
#include "heavens_gate.hpp"

namespace wow64_hook {
	/// A type represeting the tuple of (syscallID, syscall hook)
	struct HookedWow64Syscall {
		Wow64SyscallId syscall_id;
		HookedFunctionPtr hook_function_ptr;
	};

	/// An array of all the hooked functions.
	/// NOTE: All hooks MUST BE cdecl.
	extern std::array<HookedWow64Syscall, 1> g_hooked_nt_routines_arr;

	/// A hook for NtDelayExecution
	void __cdecl HookedNtDelayExecution(Wow64SyscallState state, BOOLEAN Alertable, PLARGE_INTEGER DelayInterval);
}
