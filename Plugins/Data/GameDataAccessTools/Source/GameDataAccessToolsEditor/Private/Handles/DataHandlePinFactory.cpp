// "Unreal Pokémon" created by Retro & Chill.

#include "Handles/DataHandlePinFactory.h"
#include "Handles/DataHandlePinStructPin.h"
#include "Handles/DataHandleUtils.h"

TSharedPtr<SGraphPin> FDataHandlePinFactory::CreatePin(UEdGraphPin *Pin) const
{
    if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Struct)
    {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    auto *PinStructType = Cast<UScriptStruct>(Pin->PinType.PinSubCategoryObject.Get());
    if (PinStructType == nullptr)
    {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    if (!GameData::IsDataHandleStruct(PinStructType))
    {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    return SNew(SDataHandlePinStructPin, Pin);
}