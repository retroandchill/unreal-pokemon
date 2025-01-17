// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/BattleSubsystem.h"
#include "Battle/Animations/BattleAnimationGetter.h"
#include "Lookup/InjectionUtilities.h"

void UBattleSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    AnimationGetter = UnrealInjector::NewInjectedDependency<UBattleAnimationGetter>(this);
}