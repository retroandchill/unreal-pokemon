// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/K2Node_LoadAssetByName.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetLoadingSettings.h"
#include "RetroLib/Functional/Overloaded.h"

UK2Node_LoadAssetByName::UK2Node_LoadAssetByName()
{
    SetNodeTooltip(NSLOCTEXT("UK2Node_LoadAssetByName", "NodeTooltip", "Attempts to retrieve an asset by its name"));
}

FText UK2Node_LoadAssetByName::GetNodeTitleFormat() const
{
    return NSLOCTEXT("K2Node", "LoadAssetByNameTitle", "Load {AssetClass} by Name");
}

UEdGraphPin *UK2Node_LoadAssetByName::CreateResultsPin(const FAssetClassType &AssetClass)
{
    return AssetClass.Visit(Retro::TOverloaded{
        [this](UClass *Class) {
            return CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, Class, UEdGraphSchema_K2::PN_ReturnValue);
        },
        [this](UScriptStruct *Class) {
            return CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, Class, UEdGraphSchema_K2::PN_ReturnValue);
        },
    });
}

FName UK2Node_LoadAssetByName::GetLoadFunctionName() const
{
    return GET_FUNCTION_NAME_CHECKED(UAssetLoader, LoadDynamicAsset);
}