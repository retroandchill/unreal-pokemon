// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/BattleAnimationGetter.h"
#include "Battle/BattleSubsystem.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_INJECTABLE_DEPENDENCY(UBattleAnimationGetter)

UBattleAnimationGetter &UBattleAnimationGetter::Get(const UObject *WorldContext) {
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
    check(IsValid(GameInstance))
    auto Subsystem = GameInstance->GetSubsystem<UBattleSubsystem>();
    check(IsValid(Subsystem))
    return Subsystem->GetAnimationGetter();
}