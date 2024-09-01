// "Unreal Pokémon" created by Retro & Chill.


#include "Pins/DataHandlePinFactory.h"
#include "DataManager.h"
#include "SGraphPinNameList.h"
#include "Ranges/Algorithm/ToArray.h"


TSharedPtr<SGraphPin> FDataHandlePinFactory::CreatePin(UEdGraphPin *Pin) const {
    if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Struct) {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    auto &DataManager = FDataManager::GetInstance();
    auto PinStructType = Cast<UScriptStruct>(Pin->PinType.PinSubCategoryObject.Get());
    auto StructName = PinStructType->GetMetaData("StructType");
    UScriptStruct* ReferencedType = nullptr;
    for (TObjectIterator<UScriptStruct> It; It; ++It) {
        if (It->GetName() == StructName && DataManager.HasDataTable(*It)) {
            ReferencedType = *It;
            break;
        }
    }
    if (ReferencedType == nullptr) {
        return FGraphPanelPinFactory::CreatePin(Pin);
    }

    auto StructRows = DataManager.GetDataTable(ReferencedType).GetTableRowNames();
    auto NamesList = StructRows |
        UE::Ranges::Map([](FName Name) { return MakeShared<FName>(Name).ToSharedPtr(); }) |
        UE::Ranges::ToArray;

    return SNew(SGraphPinNameList, Pin, NamesList);
}