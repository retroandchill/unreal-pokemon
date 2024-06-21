// "Unreal Pokémon" created by Retro & Chill.

#include "TestActiveSide.h"
#include "TestBattlerActor.h"
#include "Utilities/ReflectionUtils.h"

// Sets default values

ATestActiveSide::ATestActiveSide() {
    UReflectionUtils::SetPropertyValue<TSoftClassPtr<AActor>>(this, TEXT("BattlerClass"), ATestBattlerActor::StaticClass());
}