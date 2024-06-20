// "Unreal Pokémon" created by Retro & Chill.

#include "TestBattlerActor.h"
#include "TestSpriteActor.h"
#include "Utilities/ReflectionUtils.h"

ATestBattlerActor::ATestBattlerActor() {
    UReflectionUtils::SetPropertyValue<TSoftClassPtr<AActor>>(this, TEXT("BattlerSpriteClass"), ATestSpriteActor::StaticClass());
}