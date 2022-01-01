.model FLAT, C
.code

public wow64_transition_hook
extern lookup_hooked_function : proc
extern g_original_wow64_transition : dword

wow64_transition_hook PROC
	pushad
	
	push eax
	call lookup_hooked_function
	add esp, 4 ; apparently extern "C" functions are cdecl
	
	test eax, eax
	jz return
	call eax ; assuming cdecl aswell

return:
	popad
	mov edx, g_original_wow64_transition
	jmp edx
wow64_transition_hook ENDP

END