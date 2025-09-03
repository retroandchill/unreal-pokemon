// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Effects/BattleEffectUtilities.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"

void UBattleEffectUtilities::ApplyEffectToTargetWithMagnitude(const TScriptInterface<IBattler> &Target,
                                                              const TSoftClassPtr<UGameplayEffect> &EffectClass,
                                                              const FGameplayTag &DataTag, float Magnitude) {
    auto AbilityComponent = Target->GetAbilityComponent();
    auto Spec =
        AbilityComponent->MakeOutgoingSpec(EffectClass.LoadSynchronous(), 0.f, AbilityComponent->MakeEffectContext());
    Spec.Data->SetSetByCallerMagnitude(DataTag, Magnitude);
    AbilityComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data);
}