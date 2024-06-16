// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattlerActor.h"
#include "TestSpriteActor.h"
#include "External/accessor.hpp"

using namespace accessor;

MEMBER_ACCESSOR(AccessBattleSpriteActor, ABattlerActor, BattlerSpriteClass, TSoftClassPtr<AActor>)

ATestBattlerActor::ATestBattlerActor() {
    accessMember<AccessBattleSpriteActor>(*this).get() = ATestSpriteActor::StaticClass();
}