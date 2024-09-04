﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Pins/DataHandlePinFactory.h"
#include "Pins/DataHandlePinStructPin.h"
#include "Pins/PocketKeyPin.h"

TSharedPtr<SGraphPin> FDataHandlePinFactory::CreatePin(UEdGraphPin *Pin) const {
    if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Struct) {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    auto PinStructType = Cast<UScriptStruct>(Pin->PinType.PinSubCategoryObject.Get());
    check(PinStructType != nullptr)
    if (PinStructType == FPocketKey::StaticStruct()) {
        return SNew(SPocketKeyPin, Pin);
    }

    if (!Pokemon::Data::IsValidDataTableStruct(PinStructType)) {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    return SNew(SDataHandlePinStructPin, Pin);
}