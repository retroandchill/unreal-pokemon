// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * Details handler for the Grid-Based Billboard Character
 */
class GRIDBASED2DEDITOR_API FGridBasedBillboardCharacterDetails : public IDetailCustomization {
public:
 static TSharedRef<IDetailCustomization> MakeInstance();

 void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
