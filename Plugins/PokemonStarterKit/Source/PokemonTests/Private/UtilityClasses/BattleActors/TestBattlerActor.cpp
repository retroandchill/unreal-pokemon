﻿// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattlerActor.h"
#include "TestSpriteActor.h"
#include "Utilities/ReflectionUtils.h"
#include "Abilities/GameplayAbility.h"

ATestBattlerActor::ATestBattlerActor() {
    UReflectionUtils::SetPropertyValue<TSoftClassPtr<AActor>>(this, TEXT("BattlerSpriteClass"), ATestSpriteActor::StaticClass());
}

void ATestBattlerActor::PostActorCreated() {
    Super::PostActorCreated();

    // We need to add any Blueprint added effects to ensure that the data gets populated correctly
    auto &Abilities = UReflectionUtils::GetMutablePropertyValue<TArray<TSubclassOf<UGameplayAbility>>>(this, "InnateAbilities");
    auto StabEffect = LoadClass<UGameplayAbility>(nullptr, TEXT("/Game/Blueprints/Battle/Innate/Innate_SameTypeAttackBonus.Innate_SameTypeAttackBonus_C"));
    check(StabEffect != nullptr)
    Abilities.Emplace(StabEffect);
}