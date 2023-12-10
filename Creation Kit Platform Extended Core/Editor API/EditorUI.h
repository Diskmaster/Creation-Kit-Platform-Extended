// Copyright � 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class EditorUI
		{
		public:
			EditorUI();

			inline bool HasUseDeferredDialogInsert() const { return UseDeferredDialogInsert; }
			inline HWND GetDeferredListView() const { return DeferredListView; }
			inline HWND GetDeferredComboBox() const { return DeferredComboBox; }
			inline uintptr_t GetDeferredStringLength() const { return DeferredStringLength; }
			inline bool HasDeferredAllowResize() const { return DeferredAllowResize; }
			inline void SetUseDeferredDialogInsert(bool v) { UseDeferredDialogInsert = v; }
			inline void SetDeferredListView(HWND v) { DeferredListView = v; }
			inline void SetDeferredComboBox(HWND v) { DeferredComboBox = v; }
			inline void SetDeferredStringLength(uintptr_t v) { DeferredStringLength = v; }
			inline void SetDeferredAllowResize(bool v) { DeferredAllowResize = v; }
			inline Array<std::pair<const char*, void*>>& GetDeferredMenuItems() { return DeferredMenuItems; }

			void ResetUIDefer();
			void BeginUIDefer();
			void EndUIDefer();
			void SuspendComboBoxUpdates(HWND ComboHandle, bool Suspend);
		public:
			static void HKResetUIDefer();
			static void HKBeginUIDefer();
			static void HKEndUIDefer();
		private:
			EditorUI(const EditorUI&) = default;
			EditorUI& operator=(const EditorUI&) = default;

			bool UseDeferredDialogInsert;
			HWND DeferredListView;
			HWND DeferredComboBox;
			uintptr_t DeferredStringLength;
			bool DeferredAllowResize;
			Array<std::pair<const char*, void*>> DeferredMenuItems;
		};

		extern EditorUI* GlobalEditorUIPtr;
	}
}