// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Abilities/AbilityLookup.h"
#include "Abilities/GameplayAbility.h"
#include "Settings/BaseSettings.h"


TSubclassOf<UGameplayAbility> Battle::Abilities::CreateAbilityEffect(FName ID, UObject *Outer) {
    if (ID.IsNone()) {
        return nullptr;
    }
    
    auto& AssetPaths = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths();
    const auto &[ClassPath] = AssetPaths.AbilityBattleEffectPackageName;
    auto &ClassPrefix = AssetPaths.AbilityEffectPrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"),
        { ClassPath, ClassPrefix, ID.ToString() });
    if (auto AbilityClass = LoadObject<UClass>(nullptr, *FullPackage); AbilityClass != nullptr && AbilityClass->IsChildOf<UGameplayAbility>()) {
        return AbilityClass;
    }

    return nullptr;
}