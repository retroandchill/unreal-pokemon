// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

class SGraphPin;
/**
 *
 */
class POKEMONEDITORTOOLS_API FDataHandlePinFactory : public FGraphPanelPinFactory {
  public:
    FDataHandlePinFactory() = default;
    TSharedPtr<SGraphPin> CreatePin(UEdGraphPin *Pin) const override;
};
