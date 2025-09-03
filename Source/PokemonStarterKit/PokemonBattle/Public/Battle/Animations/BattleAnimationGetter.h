// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Lookup/InjectableDependency.h"
#include "UObject/Interface.h"

#include "BattleAnimationGetter.generated.h"

class IBattler;
class IBattleAnimation;
/**
 *
 */
UCLASS(Abstract, Blueprintable)
class POKEMONBATTLE_API UBattleAnimationGetter : public UObject {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    static UBattleAnimationGetter &Get(const UObject *WorldContext);

    static UBattleAnimationGetter &Get(const FScriptInterface &WorldContext) {
        return Get(WorldContext.GetObject());
    }

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Animations")
    TScriptInterface<IBattleAnimation> GetDamageAnimation(const TArray<TScriptInterface<IBattler>> &Battlers);

    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Animations")
    TScriptInterface<IBattleAnimation> GetFaintAnimation(const TScriptInterface<IBattler> &Battler);

    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    TScriptInterface<IBattleAnimation> GetRecallAnimation(const TScriptInterface<IBattler> &SwappingFrom);

    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    TScriptInterface<IBattleAnimation> GetSendOutAnimation(const TScriptInterface<IBattler> &SwappingTo,
                                                           const TScriptInterface<IBattler> &SwappingFrom);
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONBATTLE_API, UBattleAnimationGetter)