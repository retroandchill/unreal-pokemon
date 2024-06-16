// "Unreal Pokémon" created by Retro & Chill.


#include "TestActiveSide.h"
#include "TestBattlerActor.h"
#include "External/accessor.hpp"

using namespace accessor;

MEMBER_ACCESSOR(AccessBattlerClass, AActiveSide, BattlerClass, TSoftClassPtr<AActor>)

// Sets default values

ATestActiveSide::ATestActiveSide() {
    accessMember<AccessBattlerClass>(*this).get() = ATestBattlerActor::StaticClass();
}