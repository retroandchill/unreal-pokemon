// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Items/ItemLookup.h"
#include "Settings/BaseSettings.h"
#include "Abilities/GameplayAbility.h"
#include "Bag/Item.h"

TSubclassOf<UGameplayAbility> Pokemon::Battle::Items::FindHoldItemEffect(FName ID) {
    if (ID.IsNone()) {
        return nullptr;
    }
    
    auto& AssetPaths = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths();
    const auto &[ClassPath] = AssetPaths.HoldItemBattleEffectPackageName;
    auto &ClassPrefix = AssetPaths.HoldItemEffectPrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"),
        { ClassPath, ClassPrefix, ID.ToString() });
    if (auto HoldItemClass = LoadObject<UClass>(nullptr, *FullPackage); HoldItemClass != nullptr && HoldItemClass->IsChildOf<UGameplayAbility>()) {
        return HoldItemClass;
    }

    return nullptr;
}

TSubclassOf<UGameplayAbility> Pokemon::Battle::Items::FindHoldItemEffect(const FItem *Item) {
    if (Item == nullptr) {
        return nullptr;
    }

    return FindHoldItemEffect(Item->ID);
}