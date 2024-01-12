// Copyright � 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\BaseWindow.h"
#include "Editor API/SSE/TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			//using namespace CreationKitPlatformExtended::EditorAPI::Fallout4;

			class CellViewWindow : public BaseWindow, public Classes::CUIBaseWindow
			{
			public:
				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				CellViewWindow();

				static LRESULT CALLBACK HKWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
				static void sub1(HWND ListViewHandle, TESForm* Form, bool UseImage, int ItemIndex);
				static int sub2(HWND** ListViewHandle, TESForm** Form, __int64 a3);

				void ResizeWnd(UINT width, UINT height);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				Classes::CUIBaseControl m_WorldSpaceLabel;
				Classes::CUIBaseControl m_WorldSpaceComboBox;
				Classes::CUIBaseControl m_XEdit;
				Classes::CUIBaseControl m_YEdit;
				Classes::CUIBaseControl m_XLabel;
				Classes::CUIBaseControl m_YLabel;
				Classes::CUIBaseControl m_GoButton;
				Classes::CUIBaseControl m_LoadedAtTop;
				Classes::CUIBaseControl m_IdEdit;
				Classes::CUIBaseControl m_SelectedObjectLabel;
				Classes::CUIBaseControl m_ActiveCellsOnly;
				Classes::CUIBaseControl m_ActiveObjectsOnly;
				Classes::CUIBaseControl m_CellListView;
				Classes::CUIBaseControl m_ObjectListView;
			};

			extern CellViewWindow* GlobalCellViewWindowPtr;
		}
	}
}