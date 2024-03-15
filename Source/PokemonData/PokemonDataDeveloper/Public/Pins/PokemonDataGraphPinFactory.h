// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintGraphPanelPinFactory.h"
#include "EdGraphUtilities.h"

/**
 * Factory class to handling the visuals of pins for the Pokémon Data module
 */
class POKEMONDATADEVELOPER_API FPokemonDataGraphPinFactory : public FGraphPanelPinFactory {
public:
  TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override;
};
