// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattlerActor.h"
#include "External/accessor.hpp"
#include "TestSpriteActor.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessBattleSpriteActor, ABattlerActor, BattlerSpriteClass, TSoftClassPtr<AActor>)

void ATestBattlerActor::BeginPlay() {
    Super::BeginPlay();
    accessMember<AccessBattleSpriteActor>(*this).get() = ATestSpriteActor::StaticClass();
}