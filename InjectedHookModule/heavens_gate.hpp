#pragma once
#include <cstdint>
#include <unordered_map>
#include <mutex>
#include <Windows.h>
#include <iostream>
#include "Module.hpp"
#include "winapi_utils.hpp"

namespace wow64_hook {
	/// This type represents the function ID that is placed in eax 
	/// before calling Wow64SystemServiceCall.
	using Wow64SyscallId = std::uint32_t;

	/// This type represents a ptr to a Nt* hook.
	using HookedFunctionPtr = void*;

	/// This function hooks the Wow64Transition symbol from the 32bit ntdll.
	/// It also saves the old symbol value for restoration.
	void hook_wow64_transition();

	/// This function restores the original Wow64Transition.
	void unhook_wow64_transition();

	/**
	* This function is called to add a hooked Nt* routine.
	* 
	* @param syscall_id: A syscall ID conforming to the Wow64SyscallId docs.
	* @param routine_hook: The new Nt* routine. NOTE: the original routine will be called afterwards.
	*/
	void add_hook(Wow64SyscallId syscall_id, HookedFunctionPtr routine_hook);

	/**
	* This function is called from the wow64_transition_hook asm routine.
	* It translates a syscall id to the new Nt* hooked function ptr.
	* NOTE: this is not inside the "wow64_hook" namespace of of necessity:
	* we need to access this function from assembly.
	*
	* @param syscall_id: The wow64 syscall id to look up.
	* @return: nullptr if no hook was placed, else a ptr to the hook.
	*/
	extern "C" HookedFunctionPtr lookup_hooked_function(Wow64SyscallId syscall_id);

	/**
	* This struct represents the state of the processor 
	* at the time of making the heavens gate call.
	* It's used as the first parameter to every hooked function.
	*/
	struct Wow64SyscallState {
		// pusha
		std::uint32_t edi;
		std::uint32_t esi;
		std::uint32_t ebp;
		std::uint32_t esp;
		std::uint32_t ebx;
		std::uint32_t edx;
		std::uint32_t ecx;
		std::uint32_t eax;

		// return address after the translation and syscall
		void* original_ret_addr;

		// return address after ntdll
		void* wrapper_ret_addr;
	};
}
