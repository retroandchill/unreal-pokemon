// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/MoveLookup.h"
#include "Abilities/GameplayAbility.h"
#include "DynamicAssetLoadingSettings.h"
#include "PokemonBattleSettings.h"

TSubclassOf<UGameplayAbility> Pokemon::Battle::Moves::LookupMoveEffectClass(FName FunctionCode) {
    if (FunctionCode.IsNone()) {
        auto MoveClass = GetDefault<UPokemonBattleSettings>()->DefaultMoveAbility.TryLoadClass<UGameplayAbility>();
        check(MoveClass != nullptr)
        return MoveClass;
    }

    auto &AssetPaths = *GetDefault<UDynamicAssetLoadingSettings>();
    const auto &[ClassPath] = AssetPaths.MoveFunctionCodePackageName;
    auto &ClassPrefix = AssetPaths.MoveFunctionCodePrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"), {ClassPath, ClassPrefix, FunctionCode.ToString()});
    if (auto MoveClass = LoadObject<UClass>(nullptr, *FullPackage);
        MoveClass != nullptr && MoveClass->IsChildOf(UGameplayAbility::StaticClass())) {
        return MoveClass;
    }

    auto MoveClass = GetDefault<UPokemonBattleSettings>()->DefaultMoveAbility.TryLoadClass<UGameplayAbility>();
    check(MoveClass != nullptr)
    return MoveClass;
}