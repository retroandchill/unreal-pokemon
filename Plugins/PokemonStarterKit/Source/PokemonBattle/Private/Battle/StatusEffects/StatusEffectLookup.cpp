// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/StatusEffects/StatusEffectLookup.h"
#include "DynamicAssetLoadingSettings.h"
#include "GameplayEffect.h"
#include "Battle/Status.h"


TSubclassOf<UGameplayEffect> Pokemon::Battle::StatusEffects::FindStatusEffect(FName ID) {
    if (ID.IsNone()) {
        return nullptr;
    }

    auto &AssetPaths = *GetDefault<UDynamicAssetLoadingSettings>();
    const auto &[ClassPath] = AssetPaths.StatusEffectPackageName;
    auto &ClassPrefix = AssetPaths.StatusEffectPrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"), {ClassPath, ClassPrefix, ID.ToString()});
    if (auto StatusEffectClass = LoadObject<UClass>(nullptr, *FullPackage);
        StatusEffectClass != nullptr && StatusEffectClass->IsChildOf<UGameplayEffect>()) {
        return StatusEffectClass;
    }

    return nullptr;
}

TSubclassOf<UGameplayEffect> Pokemon::Battle::StatusEffects::FindStatusEffect(TOptional<const FStatus &> Status) {
    return Status.IsSet() ? FindStatusEffect(Status->ID) : nullptr;
}