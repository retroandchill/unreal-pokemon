// "Unreal Pokémon" created by Retro & Chill.

#include "TestPokemonBattle.h"
#include "TestActiveSide.h"
#include "External/accessor.hpp"

using namespace accessor;

MEMBER_ACCESSOR(AccessSideClass, APokemonBattle, BattleSideClass, TSoftClassPtr<AActor>)

ATestPokemonBattle::ATestPokemonBattle() {
    accessMember<AccessSideClass>(*this).get() = ATestActiveSide::StaticClass();
}