// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BattleStatusEffectUtils.generated.h"

class UGameplayAbility;
class IBattler;
/**
 * Helper class related to status effects
 */
UCLASS()
class POKEMONBATTLE_API UBattleStatusEffectUtils : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Check if a status effect can be inflicted on the target
     * @param StatusEffectID
     * @param Target The target of the status effect
     * @param AlreadyAppliedFormat
     * @param HasOtherStatusFormat
     * @return Can the effect be inflicted
     */
    UFUNCTION(BlueprintCallable, Category = StatusEffects)
    static bool CanStatusEffectBeInflicted(FName StatusEffectID,
                                           const TScriptInterface<IBattler>& Target, FText AlreadyAppliedFormat,
                                           FText HasOtherStatusFormat);
};
