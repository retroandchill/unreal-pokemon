// "Unreal Pokémon" created by Retro & Chill.

#include "TestPokemonBattle.h"
#include "TestActiveSide.h"
#include "Utilities/ReflectionUtils.h"

ATestPokemonBattle::ATestPokemonBattle()
{
    UReflectionUtils::SetPropertyValue<TSoftClassPtr<AActor>>(this, TEXT("BattleSideClass"),
                                                              ATestActiveSide::StaticClass());
}