// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UE5Coro.h"
#include "UObject/Interface.h"

#include "BattleAnimation.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBattleAnimation : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface used for animations played in battle
 */
class POKEMONBATTLE_API IBattleAnimation
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual UE5Coro::TCoroutine<> PlayAnimation() = 0;

    static UE5Coro::TCoroutine<> PlayAnimation(const TScriptInterface<IBattleAnimation> &Animation);
};