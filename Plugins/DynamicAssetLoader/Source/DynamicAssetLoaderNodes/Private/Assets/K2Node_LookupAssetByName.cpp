// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/K2Node_LookupAssetByName.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetLoadingSettings.h"
#include "RetroLib/Functional/Overloaded.h"
#include "RetroLib/Optionals/PtrOrNull.h"

UK2Node_LookupAssetByName::UK2Node_LookupAssetByName() {
    SetNodeTooltip(
        NSLOCTEXT("UK2Node_LookupAssetByName", "NodeTooltip", "Attempts to find the path of an asset by its name"));
}

FText UK2Node_LookupAssetByName::GetNodeTitleFormat() const {
    return NSLOCTEXT("K2Node", "LookupAssetByNameTitle", "Lookup {AssetClass} by Name");
}

UEdGraphPin *UK2Node_LookupAssetByName::CreateResultsPin(const FAssetClassType &AssetClass) {
    return AssetClass.Visit(Retro::TOverloaded{
        [this](UClass *Class) {
            return CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_SoftObject, Class, UEdGraphSchema_K2::PN_ReturnValue);
        },
        [this](UScriptStruct *Struct) {
            // clang-format off
            auto SoftStruct = Retro::FVariantObjectStructRegistry::Get().GetVariantStructData(*Struct) |
                              Retro::Optionals::Transform<&Retro::IVariantRegistration::GetSoftStructType>() |
                              Retro::Optionals::PtrOrNull;
            // clang-format on
            return CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, SoftStruct, UEdGraphSchema_K2::PN_ReturnValue);
        }});
}

FName UK2Node_LookupAssetByName::GetLoadFunctionName() const {
    return GET_FUNCTION_NAME_CHECKED(UAssetLoader, LookupDynamicAsset);
}