// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Effects/TurnBasedGameplayEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "MathUtilities.h"
#include "Battle/TurnBasedEffectComponent.h"
#include "Battle/Battlers/Battler.h"
#include "Misc/DataValidation.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OrElse.h"
#include "Ranges/Utilities/Localization.h"

bool UTurnBasedGameplayEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &GEContainer,
                                                                    FActiveGameplayEffect &ActiveGE) const {
    auto OwningActor = GEContainer.Owner->GetOwnerActor();
    auto TurnBasedComponent = OwningActor->FindComponentByClass<UTurnBasedEffectComponent>();
    check(IsValid(TurnBasedComponent))
    // clang-format off
    int32 Duration = TurnDuration |
                     UE::Optionals::Map(&UMathUtilities::RandomIntInRange) |
                     UE::Optionals::OrElse(INDEX_NONE);
    // clang-format on
    TurnBasedComponent->AddTurnBasedEffect(ActiveGE.Handle, Duration);
    auto EventSet = GEContainer.Owner->GetActiveEffectEventSet(ActiveGE.Handle);
    EventSet->OnEffectRemoved.AddStatic(&UTurnBasedGameplayEffectComponent::OnGameplayEffectRemoved,
                                        TurnBasedComponent);
    return Super::OnActiveGameplayEffectAdded(GEContainer, ActiveGE);
}

#if WITH_EDITOR
EDataValidationResult UTurnBasedGameplayEffectComponent::IsDataValid(FDataValidationContext &Context) const {
    auto OriginalResult = Super::IsDataValid(Context);

    bool bHasError = false;
    if (TurnDuration.IsSet()) {
        auto &Range = TurnDuration.GetValue();
        if (!Range.HasLowerBound()) {
            Context.AddError(LOCALIZED_TEXT("BadLowerBound", "Turn Duration must have a lower bound"));
            bHasError = true;
        } else if (Range.GetLowerBoundValue() <= 0) {
            Context.AddError(LOCALIZED_TEXT("BadLowerBoundValue", "Turn Duration must have a positive lower bound"));
            bHasError = true;
        }

        if (!Range.HasUpperBound()) {
            Context.AddError(LOCALIZED_TEXT("BadUpperBound", "Turn Duration must have an upper bound"));
            bHasError = true;
        } else if (Range.GetUpperBoundValue() <= 0) {
            Context.AddError(LOCALIZED_TEXT("BadUpperBoundValue", "Turn Duration must have a positive upper bound"));
            bHasError = true;
        }
    }

    return bHasError ? EDataValidationResult::Invalid : OriginalResult;
}
#endif

void UTurnBasedGameplayEffectComponent::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo &GERemovalInfo,
                                                                UTurnBasedEffectComponent *Component) {
    Component->RemoveTurnBasedEffect(GERemovalInfo.ActiveEffect->Handle);
}