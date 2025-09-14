// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "AssetTypeActions_Base.h"

namespace Simple2D
{
    /**
     *
     */
    class SIMPLE2DEDITOR_API FSimpleFlipbookAssetActions : public FAssetTypeActions_Base
    {
      public:
        FText GetName() const override;
        UClass *GetSupportedClass() const override;
        FColor GetTypeColor() const override;
        uint32 GetCategories() override;
        void OpenAssetEditor(const TArray<UObject *> &InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor =
                                                                     TSharedPtr<IToolkitHost>()) override;
    };
} // namespace Simple2D
