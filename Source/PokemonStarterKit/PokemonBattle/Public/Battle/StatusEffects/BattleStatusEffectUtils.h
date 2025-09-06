// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Battle/Status.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UE5Coro.h"

#include "BattleStatusEffectUtils.generated.h"

class UGameplayAbility;
class IBattler;
/**
 * Helper class related to status effects
 */
UCLASS()
class POKEMONBATTLE_API UBattleStatusEffectUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Check if a status effect can be inflicted on the target
     * @param Target The target of the status effect
     * @param StatusEffectID
     * @param AlreadyAppliedFormat
     * @param HasOtherStatusFormat
     * @param
     * @return Can the effect be inflicted
     */
    static UE5Coro::TCoroutine<bool> CanStatusEffectBeInflicted(const TScriptInterface<IBattler> &Target,
                                                                FName StatusEffectID, const FText &AlreadyAppliedFormat,
                                                                const FText &HasOtherStatusFormat,
                                                                FForceLatentCoroutine = {});

    static UE5Coro::TCoroutine<FActiveGameplayEffectHandle> ApplyStatusEffectToBattler(
        const TScriptInterface<IBattler> &Battler, FStatusHandle StatusEffect, FForceLatentCoroutine = {});

    static UE5Coro::TCoroutine<bool> RemoveStatusEffectFromBattler(const TScriptInterface<IBattler> &Target,
                                                                   FForceLatentCoroutine = {});

    static UE5Coro::TCoroutine<bool> RemoveStatusEffectFromBattler(const TScriptInterface<IBattler> &Target,
                                                                   FStatusHandle StatusEffect,
                                                                   FForceLatentCoroutine = {});
};
