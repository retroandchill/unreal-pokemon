// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Graphics/BattleRender.h"
#include "UObject/Object.h"

#include "ExceptionTestHelper.generated.h"

struct FBattleRender;
/**
 *
 */
UCLASS()
class POKEMONTESTS_API UExceptionTestHelper : public UObject
{
    GENERATED_BODY()

  public:
    UPROPERTY()
    FSlateBrush Brush;

    UPROPERTY()
    FBattleRender Render;
};
