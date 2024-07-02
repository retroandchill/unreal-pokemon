// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Events/Moves/CriticalHitRateCalculationPayload.h"
#include "Species/SpeciesData.h"

ECriticalOverride UMoveEvaluationHelpers::ApplyCriticalHitOverride(ECriticalOverride Old, ECriticalOverride New) {
    using enum ECriticalOverride;
    if (Old == Normal) {
        return New;
    }

    if (Old == Always && New == Never) {
        return New;
    }

    // If the old value is set to Never, then we go with that, no questions asked
    return Old;
}

void UMoveEvaluationHelpers::SetCriticalHitOverride(const UCriticalHitRateCalculationPayload *Context,
                                                    ECriticalOverride Override) {
    auto &Data = Context->GetData();
    Context->SetCriticalHitRateOverride(ApplyCriticalHitOverride(Data.Override, Override));
}

bool UMoveEvaluationHelpers::UserIsSpecies(const TScriptInterface<IMoveEventPayload> &Context, FName Species) {
    return Context->GetUser()->GetSpecies().ID == Species;
}

void UMoveEvaluationHelpers::BoostPowerIfUserHasTag(const UDamageModificationPayload *Context, FGameplayTag Tag,
                                                    float Multiplier) {
    auto &Data = Context->GetData();
    if (Data.User->GetAbilityComponent()->HasMatchingGameplayTag(Tag)) {
        Context->SetPowerMultiplier(Data.PowerMultiplier * Multiplier);
    }
}

void UMoveEvaluationHelpers::BoostPowerIfUserHasAllTags(const UDamageModificationPayload *Context,
    FGameplayTagContainer Tags, float Multiplier) {
    auto &Data = Context->GetData();
    if (Data.User->GetAbilityComponent()->HasAllMatchingGameplayTags(Tags)) {
        Context->SetPowerMultiplier(Data.PowerMultiplier * Multiplier);
    }
}

void UMoveEvaluationHelpers::BoostPowerIfUserHasAnyTags(const UDamageModificationPayload *Context,
    FGameplayTagContainer Tags, float Multiplier) {
        auto &Data = Context->GetData();
        if (Data.User->GetAbilityComponent()->HasAnyMatchingGameplayTags(Tags)) {
            Context->SetPowerMultiplier(Data.PowerMultiplier * Multiplier);
        }
}