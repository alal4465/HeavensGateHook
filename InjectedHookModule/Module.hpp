#pragma once
#include <string>
#include <Windows.h>
#include "UniqueHandle.hpp"
#include "Exceptions.hpp"

namespace wow64_hook {
	namespace winapi {
		class Module final {
		public:
			/// A 'using' statement for module handles (wraps HMODULE).
			using ModuleHandle = UniqueHandle<HMODULE, void(*)(HMODULE)>;

			/**
			* Open and wrap a handle to the module.
			* 
			* @param module_name: The module to open.
			*/
			Module(const std::string& module_name);

			/**
			* Lookup a symbol in the module (wraps GetProcAddress).
			* @param: The symbol to look up.
			* @return: A ptr to the symbol.
			*/
			void* lookup_symbol(const std::string& symbol_name);

		private:
			/**
			* A function wrapper for GetModuleHandleA
			* for taking adventage of initializer lists in the constructor.
			* 
			* @param module_name: The name of the (already loaded) module.
			* @return: A handle wrapper for the HMODULE of the underlying module.
			*/
			static ModuleHandle get_module_handle(const std::string& module_name);

			/// A handle to the opened module
			ModuleHandle module_handle_;
		};
	}
}