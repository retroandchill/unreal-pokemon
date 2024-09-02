// "Unreal Pokémon" created by Retro & Chill.


#include "Pins/DataHandlePinFactory.h"
#include "DataManager.h"
#include "SGraphPinNameList.h"
#include "Pins/DataHandlePinStructPin.h"
#include "Ranges/Algorithm/ToArray.h"


TSharedPtr<SGraphPin> FDataHandlePinFactory::CreatePin(UEdGraphPin *Pin) const {
    if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Struct) {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    auto PinStructType = Cast<UScriptStruct>(Pin->PinType.PinSubCategoryObject.Get());
    check(PinStructType != nullptr)
    if (!Pokemon::Data::IsValidDataTableStruct(PinStructType)) {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    return SNew(SDataHandlePinStructPin, Pin);
}