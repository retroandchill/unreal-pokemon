// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Battlers/BattlerSprite.h"
#include "GameFramework/Actor.h"

#include "TestSpriteActor.generated.h"

UCLASS(BlueprintInternalUseOnly)
class POKEMONTESTS_API ATestSpriteActor : public AActor, public IBattlerSprite
{
    GENERATED_BODY()
};