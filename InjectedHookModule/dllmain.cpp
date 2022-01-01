#include <Windows.h>
#include "nt_hooks.hpp"
#include "heavens_gate.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        wow64_hook::hook_wow64_transition();
        for (auto&& [syscall_id, hook] : wow64_hook::g_hooked_nt_routines_arr) {
            wow64_hook::add_hook(syscall_id, hook);
        }
    } else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        wow64_hook::unhook_wow64_transition();
    }

    return TRUE;
}

