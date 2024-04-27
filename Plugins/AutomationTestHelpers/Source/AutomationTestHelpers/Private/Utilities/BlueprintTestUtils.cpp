// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/BlueprintTestUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"

UClass *UBlueprintTestUtils::LoadBlueprintClassByName(FStringView ClassName) {
    auto WidgetBlueprintAsset = FAssetRegistryModule::GetRegistry()
        .GetAssetByObjectPath(FSoftObjectPath(ClassName));
    auto WidgetBlueprint = Cast<UBlueprint>(WidgetBlueprintAsset.GetAsset());
    if (WidgetBlueprint == nullptr) {
        return nullptr;
    }
    
    return WidgetBlueprint->GeneratedClass.Get();
}