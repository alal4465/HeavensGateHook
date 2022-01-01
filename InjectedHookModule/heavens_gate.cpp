#include "heavens_gate.hpp"


namespace wow64_hook {
	/**
	* Declerations with C linkage.
	*/
	extern "C" {
		/// The original Wow64Transition symbol value in the 32bit ntdll.dll.
		unsigned long g_original_wow64_transition = 0;

		/// The assembly routine hooking the Wow64Transition symbol.
		void wow64_transition_hook();
	}

	/// This unordered map translates from a syscall id to our hooked function.
	static std::unordered_map<Wow64SyscallId, HookedFunctionPtr> g_hooked_wow64_nt_routines;

	/// A mutex protecting the g_hooked_wow64_nt_routines map.
	static std::mutex g_wow64_hook_mutex;

	void hook_wow64_transition() {
		if (g_original_wow64_transition != 0) {
			return;
		}

		winapi::Module ntdll("ntdll.dll");
		auto wow64_transition_ptr = ntdll.lookup_symbol("Wow64Transition");
		winapi::mem_protect(wow64_transition_ptr, sizeof(void*), winapi::MemoryProtection::ReadWrite);

		g_original_wow64_transition = InterlockedExchange(
			reinterpret_cast<unsigned long*>(wow64_transition_ptr), 
			reinterpret_cast<unsigned long>(&wow64_transition_hook)
		);
	}

	void unhook_wow64_transition() {
		winapi::Module ntdll("ntdll.dll");
		auto wow64_transition_ptr = ntdll.lookup_symbol("Wow64Transition");

		InterlockedExchange(
			reinterpret_cast<unsigned long*>(wow64_transition_ptr),
			g_original_wow64_transition
		);
	}

	void add_hook(Wow64SyscallId syscall_id, HookedFunctionPtr routine_hook) {
		std::lock_guard<std::mutex> lock(g_wow64_hook_mutex);
		g_hooked_wow64_nt_routines[syscall_id] = routine_hook;
	}

	extern "C" HookedFunctionPtr lookup_hooked_function(Wow64SyscallId syscall_id) {
		std::lock_guard<std::mutex> lock(g_wow64_hook_mutex);
		return g_hooked_wow64_nt_routines.find(syscall_id) != g_hooked_wow64_nt_routines.end() ?
			g_hooked_wow64_nt_routines.at(syscall_id) : nullptr;
	}
}
