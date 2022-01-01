#include "Module.hpp"

namespace wow64_hook {
	namespace winapi {
		Module::Module(const std::string& module_name) :
			module_handle_(get_module_handle(module_name)) {}

		void* Module::lookup_symbol(const std::string& symbol_name) {
			auto symbol = ::GetProcAddress(module_handle_.get(), symbol_name.c_str());
			if (symbol == nullptr) {
				throw GetProcAddressException();
			}

			return symbol;
		}

		Module::ModuleHandle Module::get_module_handle(const std::string& module_name) {
			auto raw_handle = ::GetModuleHandleA(module_name.c_str());
			if (raw_handle == nullptr) {
				throw GetModuleHandleException();
			}

			// Does not close the handle.
			// This is valid behaviour: https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlea.
			return ModuleHandle(raw_handle, [](HMODULE) {});
		}
	}
}