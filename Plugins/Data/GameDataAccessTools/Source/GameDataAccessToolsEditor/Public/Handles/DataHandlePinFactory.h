// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

class SGraphPin;

/**
 * A pin factory class for creating custom pins related to data handle structures in the Unreal Engine editor.
 *
 * This class overrides the FGraphPanelPinFactory base class to provide a specialized implementation for creating
 * pins that represent GameDataHandle structures. It is intended to support the graphical representation and
 * interactivity of custom pins within the Unreal Engine editor's graph panels.
 */
class GAMEDATAACCESSTOOLSEDITOR_API FDataHandlePinFactory : public FGraphPanelPinFactory
{
  public:
    FDataHandlePinFactory() = default;
    TSharedPtr<SGraphPin> CreatePin(UEdGraphPin *Pin) const override;
};
