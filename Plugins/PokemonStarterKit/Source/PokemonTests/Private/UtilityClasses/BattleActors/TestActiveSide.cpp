// "Unreal Pokémon" created by Retro & Chill.

#include "TestActiveSide.h"
#include "External/accessor.hpp"
#include "TestBattlerActor.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessBattlerClass, AActiveSide, BattlerClass, TSoftClassPtr<AActor>)

// Sets default values

void ATestActiveSide::BeginPlay() {
    Super::BeginPlay();
    accessMember<AccessBattlerClass>(*this).get() = ATestBattlerActor::StaticClass();
}