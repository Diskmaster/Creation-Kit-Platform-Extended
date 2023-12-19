// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiSkinInstance.h

#pragma once

#include "NiObject.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiSkinInstance : public NiObject
		{
		public:
			class NiSkinData* m_spSkinData;
			void* m_spSkinPartition;
			char _pad0[0x28];
			void* m_pvBoneMatrices;
			void* m_pvPrevBoneMatrices;
			char _pad1[0x8];
			CRITICAL_SECTION m_csLock;

			virtual ~NiSkinInstance();
		};
		static_assert(sizeof(NiSkinInstance) == 0x88);
	}
}