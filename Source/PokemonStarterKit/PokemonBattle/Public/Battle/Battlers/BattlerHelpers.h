// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Battler.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utilities/StatusEffectUtilities.h"

#include "BattlerHelpers.generated.h"

struct FExpGainInfo;
class IBattler;
struct FGameplayAbilityActorInfo;

/**
 * Library of helper functions used for battlers
 */
UCLASS()
class POKEMONBATTLE_API UBattlerHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Get the value of a battler's stat for the given attribute
     * @param ActorInfo The information about the actor in question
     * @param Attribute The attribute to find
     * @param bFound Was the attribute in question found
     * @return The value of the attribute
     */
    UFUNCTION(BlueprintPure, Category = "Battler|Stats")
    static float GetBattlerStat(const FGameplayAbilityActorInfo &ActorInfo, FGameplayAttribute Attribute, bool &bFound);

    /**
     * Get the Pokémon's status effect
     * @param Battler The Pokémon to get the status effect for
     * @param StatusEffect The status effect information to write that data into
     * @return If the Pokémon has a status effect
     */
    UFUNCTION(BlueprintCallable, Category = "Battler|StatusEffects",
              meta = (ExpandEnumAsExecs = "ReturnValue", DefaultToSelf = "Battler"))
    static EStatusEffectStatus GetStatusEffect(const TScriptInterface<IBattler> &Battler,
                                               FStatusEffectInfo &StatusEffect);

    static UE5Coro::TCoroutine<> GainExpOnFaint(UE5Coro::TLatentContext<const UObject> Context,
                                                const TArray<TScriptInterface<IBattler>> &FainedBattlers);

    static UE5Coro::TCoroutine<bool> ApplyHPRecoveryEffect(const TScriptInterface<IBattler> &Battler, int32 Amount,
                                                           const UGameplayAbility *Ability = nullptr,
                                                           bool bShowFailureMessage = false,
                                                           FForceLatentCoroutine = {});
};
