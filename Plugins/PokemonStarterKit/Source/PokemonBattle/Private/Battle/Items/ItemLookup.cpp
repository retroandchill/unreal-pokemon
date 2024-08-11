// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Items/ItemLookup.h"
#include "Abilities/GameplayAbility.h"
#include "Bag/Item.h"
#include "DynamicAssetLoadingSettings.h"
#include "PokemonBattleSettings.h"

TSubclassOf<UGameplayAbility> Pokemon::Battle::Items::FindHoldItemEffect(FName ID) {
    if (ID.IsNone()) {
        return nullptr;
    }

    auto &AssetPaths = *GetDefault<UDynamicAssetLoadingSettings>();
    const auto &[ClassPath] = AssetPaths.HoldItemBattleEffectPackageName;
    auto &ClassPrefix = AssetPaths.HoldItemEffectPrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"), {ClassPath, ClassPrefix, ID.ToString()});
    if (auto HoldItemClass = LoadObject<UClass>(nullptr, *FullPackage);
        HoldItemClass != nullptr && HoldItemClass->IsChildOf<UGameplayAbility>()) {
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

TSubclassOf<UGameplayAbility> Pokemon::Battle::Items::FindBattleItemEffect(FName ID) {
    if (ID.IsNone()) {
        auto MoveClass =
            GetDefault<UPokemonBattleSettings>()->DefaultBattleItemAbility.TryLoadClass<UGameplayAbility>();
        check(MoveClass != nullptr)
        return MoveClass;
    }

    auto &AssetPaths = *GetDefault<UDynamicAssetLoadingSettings>();
    const auto &[ClassPath] = AssetPaths.BattleItemEffectPackageName;
    auto &ClassPrefix = AssetPaths.BattleItemEffectPrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"), {ClassPath, ClassPrefix, ID.ToString()});
    if (auto MoveClass = LoadObject<UClass>(nullptr, *FullPackage);
        MoveClass != nullptr && MoveClass->IsChildOf(UGameplayAbility::StaticClass())) {
        return MoveClass;
    }

    auto MoveClass = GetDefault<UPokemonBattleSettings>()->DefaultBattleItemAbility.TryLoadClass<UGameplayAbility>();
    check(MoveClass != nullptr)
    return MoveClass;
}

TSubclassOf<UGameplayAbility> Pokemon::Battle::Items::FindBattleItemEffect(const FItem *Item) {
    if (Item == nullptr) {
        return FindBattleItemEffect(NAME_None);
    }

    return FindBattleItemEffect(Item->ID);
}