﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/MoveLookup.h"
#include "Abilities/GameplayAbility.h"
#include "Settings/BaseSettings.h"

TSubclassOf<UGameplayAbility> Pokemon::Battle::Moves::LookupMoveEffectClass(FName FunctionCode) {
    if (FunctionCode.IsNone()) {
        auto MoveClass = FBaseSettings::Get().GetDefaultMoveAbility().TryLoadClass<UGameplayAbility>();
        check(MoveClass != nullptr)
        return MoveClass;
    }

    auto &AssetPaths = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths();
    const auto &[ClassPath] = AssetPaths.MoveFunctionCodePackageName;
    auto &ClassPrefix = AssetPaths.MoveFunctionCodePrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"), {ClassPath, ClassPrefix, FunctionCode.ToString()});
    if (auto MoveClass = LoadObject<UClass>(nullptr, *FullPackage);
        MoveClass != nullptr && MoveClass->IsChildOf(UGameplayAbility::StaticClass())) {
        return MoveClass;
    }

    auto MoveClass = FBaseSettings::Get().GetDefaultMoveAbility().TryLoadClass<UGameplayAbility>();
    check(MoveClass != nullptr)
    return MoveClass;
}