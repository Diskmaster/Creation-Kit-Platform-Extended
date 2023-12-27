﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class AboutWindow
		{
		public:
			static INT_PTR CALLBACK WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
			static INT_PTR CALLBACK WndProc2(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
		};
	}
}