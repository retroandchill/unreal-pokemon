// "Unreal Pokémon" created by Retro & Chill.

#include "TestPokemonBattle.h"
#include "External/accessor.hpp"
#include "TestActiveSide.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessSideClass, APokemonBattle, BattleSideClass, TSoftClassPtr<AActor>)

void ATestPokemonBattle::BeginPlay() {
    Super::BeginPlay();
    accessMember<AccessSideClass>(*this).get() = ATestActiveSide::StaticClass();
}