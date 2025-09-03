// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattlerActor.h"
#include "Abilities/GameplayAbility.h"
#include "TestSpriteActor.h"
#include "Utilities/ReflectionUtils.h"

ATestBattlerActor::ATestBattlerActor() {
    UReflectionUtils::SetPropertyValue<TSoftClassPtr<AActor>>(this, TEXT("BattlerSpriteClass"),
                                                              ATestSpriteActor::StaticClass());
}

void ATestBattlerActor::PostActorCreated() {
    Super::PostActorCreated();

    // We need to add any Blueprint added effects to ensure that the data gets populated correctly
    auto &Abilities =
        UReflectionUtils::GetMutablePropertyValue<TArray<TSubclassOf<UGameplayAbility>>>(this, "InnateAbilities");
    auto StabEffect = LoadClass<UGameplayAbility>(
        nullptr, TEXT("/Game/Blueprints/Battle/Innate/Innate_SameTypeAttackBonus.Innate_SameTypeAttackBonus_C"));
    check(StabEffect != nullptr)
    Abilities.Emplace(StabEffect);
    auto HighCritMoveEffect = LoadClass<UGameplayAbility>(
        nullptr, TEXT("/Game/Blueprints/Battle/Innate/Innate_HighCriticalHitRate.Innate_HighCriticalHitRate_C"));
    check(HighCritMoveEffect != nullptr)
    Abilities.Emplace(HighCritMoveEffect);

    auto &Effects =
        UReflectionUtils::GetMutablePropertyValue<TArray<TSubclassOf<UGameplayEffect>>>(this, "InnateEffects");
    auto StatStages = LoadClass<UGameplayEffect>(
        nullptr, TEXT("/Game/Blueprints/Battle/Effects/Effect_StatStages.Effect_StatStages_C"));
    check(StatStages != nullptr)
    Effects.Emplace(StatStages);
}

UE5Coro::TCoroutine<> ATestBattlerActor::SpawnSpriteActor(bool ShouldShow) {
    co_return;
}