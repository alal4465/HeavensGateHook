#include <iostream>
#include <Windows.h>

constexpr std::string_view INJECTED_DLL = "C:\\Users\\alon\\source\\repos\\HeavensGateHook\\Debug\\InjectedHookModule.dll";

/// Quick and dirty injector code.
/// The decent(ish) code is in the dll itself.
int main(int argc, char* argv[]) {
	std::uint32_t pid = 0;
	std::cout << "Enter pid: ";
	std::cin >> pid;

	auto process_handle = ::OpenProcess(
		PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD,
		false, 
		pid
	);
	if (process_handle == INVALID_HANDLE_VALUE) {
		std::cout << "OpenProcess failed with: " << GetLastError();
		return -1;
	}

	auto alloced_dll_path = ::VirtualAllocEx(
		process_handle, 
		nullptr, 
		INJECTED_DLL.size() + 1, 
		MEM_RESERVE | MEM_COMMIT, 
		PAGE_READWRITE
	);

	if (!alloced_dll_path) {
		std::cout << "VirtualAllocEx failed with: " << GetLastError();
		::CloseHandle(process_handle);
		return -1;
	}

	SIZE_T bytes_written = 0;
	if (!::WriteProcessMemory(
		process_handle, 
		alloced_dll_path, 
		INJECTED_DLL.data(), 
		INJECTED_DLL.size() + 1, 
		&bytes_written) || bytes_written != INJECTED_DLL.size() + 1) {
		std::cout << "WriteProcessMemory failed with: " << GetLastError();
		::CloseHandle(process_handle);
		::VirtualFreeEx(process_handle, alloced_dll_path, 0, MEM_RELEASE);
		return -1;
	}

	auto thread_handle = ::CreateRemoteThread(
		process_handle, 
		nullptr, 
		0, 
		reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), 
		alloced_dll_path, 
		0, 
		nullptr
	);

	if (thread_handle) {
		::WaitForSingleObject(thread_handle, INFINITE);
	} else {
		std::cout << "CreateRemoteThread failed with: " << GetLastError();
	}
	
	::VirtualFreeEx(process_handle, alloced_dll_path, 0, MEM_RELEASE);
	::CloseHandle(process_handle);

	return 0;
}
