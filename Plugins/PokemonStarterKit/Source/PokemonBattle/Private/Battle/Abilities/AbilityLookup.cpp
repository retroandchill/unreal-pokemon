// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Abilities/AbilityLookup.h"
#include "DynamicAssetLoadingSettings.h"
#include "Abilities/GameplayAbility.h"

TSubclassOf<UGameplayAbility> Battle::Abilities::CreateAbilityEffect(FName ID) {
    if (ID.IsNone()) {
        return nullptr;
    }

    auto &AssetPaths = *GetDefault<UDynamicAssetLoadingSettings>();
    const auto &[ClassPath] = AssetPaths.AbilityBattleEffectPackageName;
    auto &ClassPrefix = AssetPaths.AbilityEffectPrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"), {ClassPath, ClassPrefix, ID.ToString()});
    if (auto AbilityClass = LoadObject<UClass>(nullptr, *FullPackage);
        AbilityClass != nullptr && AbilityClass->IsChildOf<UGameplayAbility>()) {
        return AbilityClass;
    }

    return nullptr;
}