// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Events/Moves/AdditionalEffectChanceModificationPayload.h"
#include "Battle/Events/Moves/CriticalHitRateCalculationPayload.h"
#include "Species/SpeciesData.h"

void UMoveEvaluationHelpers::IncrementCriticalHitRate(const UCriticalHitRateCalculationPayload *Context, int32 Amount)
{
    Context->SetCriticalHitRateStages(Context->GetData().CriticalHitRateStages + Amount);
}

void UMoveEvaluationHelpers::IncrementCriticalHitRateIfUserHasTag(const UCriticalHitRateCalculationPayload *Context,
                                                                  FGameplayTag Tag, int32 Amount)
{
    if (Context->GetUser()->GetAbilityComponent()->HasMatchingGameplayTag(Tag))
    {
        Context->SetCriticalHitRateStages(Context->GetData().CriticalHitRateStages + Amount);
    }
}

ECriticalOverride UMoveEvaluationHelpers::ApplyCriticalHitOverride(ECriticalOverride Old, ECriticalOverride New)
{
    using enum ECriticalOverride;
    if (Old == Normal)
    {
        return New;
    }

    if (Old == Always && New == Never)
    {
        return New;
    }

    // If the old value is set to Never, then we go with that, no questions asked
    return Old;
}

void UMoveEvaluationHelpers::SetCriticalHitOverride(const UCriticalHitRateCalculationPayload *Context,
                                                    ECriticalOverride Override)
{
    auto &Data = Context->GetData();
    Context->SetCriticalHitRateOverride(ApplyCriticalHitOverride(Data.Override, Override));
}

bool UMoveEvaluationHelpers::UserIsSpecies(const TScriptInterface<IMoveEventPayload> &Context, FName Species)
{
    return Context->GetUser()->GetSpecies().ID == Species;
}

void UMoveEvaluationHelpers::BoostPower(const UDamageModificationPayload *Context, float Multiplier)
{
    Context->SetPowerMultiplier(Context->GetData().PowerMultiplier * Multiplier);
}

void UMoveEvaluationHelpers::BoostPowerIfUserHasTag(const UDamageModificationPayload *Context, FGameplayTag Tag,
                                                    float Multiplier)
{
    if (Context->GetUser()->GetAbilityComponent()->HasMatchingGameplayTag(Tag))
    {
        BoostPower(Context, Multiplier);
    }
}

void UMoveEvaluationHelpers::BoostPowerIfUserHasAllTags(const UDamageModificationPayload *Context,
                                                        FGameplayTagContainer Tags, float Multiplier)
{
    if (Context->GetUser()->GetAbilityComponent()->HasAllMatchingGameplayTags(Tags))
    {
        BoostPower(Context, Multiplier);
    }
}

void UMoveEvaluationHelpers::BoostPowerIfUserHasAnyTags(const UDamageModificationPayload *Context,
                                                        FGameplayTagContainer Tags, float Multiplier)
{
    if (Context->GetUser()->GetAbilityComponent()->HasAnyMatchingGameplayTags(Tags))
    {
        BoostPower(Context, Multiplier);
    }
}

void UMoveEvaluationHelpers::BoostFinalDamageMultiplier(const UDamageModificationPayload *Context, float Multiplier)
{
    Context->SetPowerMultiplier(Context->GetData().FinalDamageMultiplier * Multiplier);
}

void UMoveEvaluationHelpers::BoostAdditionalEffectChance(const UAdditionalEffectChanceModificationPayload *Context,
                                                         float Multiplier)
{
    Context->SetAdditionalEffectChance(Context->GetData().AdditionalEffectChance * Multiplier);
}