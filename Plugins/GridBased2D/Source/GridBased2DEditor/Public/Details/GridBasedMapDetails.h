// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * The layout details for a grid based map
 */
class GRIDBASED2DEDITOR_API FGridBasedMapDetails : public IDetailCustomization {
  public:
    static TSharedRef<IDetailCustomization> MakeInstance();

    void CustomizeDetails(IDetailLayoutBuilder &DetailBuilder) override;
};
