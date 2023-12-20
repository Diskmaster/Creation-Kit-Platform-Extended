﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "SkipTopicInfoValidation.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		SkipTopicInfoValidationPatch::SkipTopicInfoValidationPatch() : Module(GlobalEnginePtr)
		{}

		bool SkipTopicInfoValidationPatch::HasOption() const
		{
			return true;
		}

		bool SkipTopicInfoValidationPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* SkipTopicInfoValidationPatch::GetOptionName() const
		{
			return "CreationKit:bSkipTopicInfoValidation";
		}

		const char* SkipTopicInfoValidationPatch::GetName() const
		{
			return "Skip Topic Info Validation";
		}

		bool SkipTopicInfoValidationPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
		}

		bool SkipTopicInfoValidationPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (lpRelocationDatabaseItem->Version() == 1)
			{
				//
				// Skip 'Topic Info' validation during load
				//
				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xC3 });

				return true;
			}

			return false;
		}

		bool SkipTopicInfoValidationPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}