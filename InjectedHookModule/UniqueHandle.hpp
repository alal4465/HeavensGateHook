#pragma once
#include <Windows.h>
#include <memory>

namespace wow64_hook {
	namespace winapi {

		/**
		* This class is a std::unique_ptr type wrapper for winapi handle types.
		* By default, it'll wrap HANDLE with CloseHandle.
		* It only allows for move construction and it protects the underlying handle
		* to have only a single owner (same as std::unique_ptr).
		* 
		* @tparam THandle: The type of the handle to wrap.
		* @tparam TCloseHandleFunc: The type of the callable that closes the handle.
		*/
		template <typename THandle = HANDLE, typename TCloseHandleFunc = decltype(&::CloseHandle)>
		class UniqueHandle final {
		public:
			/**
			* Construct a UniqueHandle.
			* 
			* @param raw_handle: The raw handle to wrap.
			* @param close_handle_func: The function to close the handle.
			*/
			UniqueHandle(THandle raw_handle, TCloseHandleFunc close_handle_func = ::CloseHandle):
			raw_handle_(raw_handle),
			close_handle_func_(close_handle_func) {}

			/// A deleted copy constructor.
			UniqueHandle(UniqueHandle&) = delete;
			
			/// A move constructor.
			/// repleaces the moved UniqueHandle raw handle with INVALID_HANDLE_VALUE.
			UniqueHandle(UniqueHandle&& other) {
				if (&other == this) {
					return;
				}
				move_members_from_object(other);
			}

			/// A deleted copy assignment operator
			UniqueHandle& operator=(UniqueHandle&) = delete;

			/// A move assignment operator, Works like the move constructor.
			UniqueHandle& operator=(UniqueHandle&& other) {
				if (&other == this) {
					return;
				}

				close_handle();
				move_members_from_object(other);
			}

			/// Release the underlying resource via close_handle_func.
			/// Uses the common function close_handle.
			~UniqueHandle() {
				try {
					close_handle();
				}
				catch (...) {}
			}

			/// Get the raw handle to the underlying resource.
			THandle get() const noexcept {
				return raw_handle_;
			}
		private:
			/// Release the underlying resource via close_handle_func.
			void close_handle() {
				if (raw_handle_ != INVALID_HANDLE_VALUE) {
					close_handle_func_(raw_handle_);
				}
			}

			/// A common function for the memberwise move in the move assignment and constructor.
			void move_members_from_object(UniqueHandle&& other) {
				raw_handle_ = std::move(other.raw_handle_);
				close_handle_func_ = std::move(other.close_handle_func_);
				other.raw_handle_ = INVALID_HANDLE_VALUE;
			}

			/// The raw handle for the underlying resource.
			THandle raw_handle_;

			/// A callable type to close the raw handle.
			TCloseHandleFunc close_handle_func_;
		};

	}
}
