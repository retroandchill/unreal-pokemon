// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonBattlePanel.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Components/EnhancedImage.h"
#include "Components/ProgressBar.h"
#include "Graphics/GraphicsAssetClasses.h"
#include "Utilities/PokemonUIUtils.h"

using FUpdatePercent = Pokemon::UI::FSetNewPercent::FDelegate;
using FUpdateComplete = Pokemon::UI::FOnAnimationComplete::FDelegate;

void UPokemonBattlePanel::NativeConstruct() {
    Super::NativeConstruct();
    HPBarUpdateAnimation.SetWorldContext(this);
    HPBarUpdateAnimation.BindActionToPercentDelegate(
        FUpdatePercent::CreateUObject(this, &UPokemonBattlePanel::UpdateHPPercent));
    HPBarUpdateAnimation.BindActionToCompleteDelegate(
        FUpdateComplete::CreateUObject(this, &UPokemonBattlePanel::HPPercentUpdateComplete));
}

void UPokemonBattlePanel::SetBattler(const TScriptInterface<IBattler> &Battler) {
    CurrentBattler = Battler;
    Refresh();
}

void UPokemonBattlePanel::Refresh() {
    check(CurrentBattler != nullptr)
    if (CurrentBattler->IsFainted()) {
        SetVisibility(ESlateVisibility::Hidden);
    } else {
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    RefreshStatusEffect();
    HPBar->SetPercent(CurrentBattler->GetHPPercent());

    OnRefresh();
}

void UPokemonBattlePanel::BindToOnProgressBarUpdateComplete(const FOnProgresBarUpdateComplete::FDelegate &Binding) {
    OnHPBarUpdated.Add(Binding);
}

void UPokemonBattlePanel::UnbindAllHPUpdateDelegates(UObject *Object) {
    OnHPBarUpdated.RemoveAll(Object);
}

void UPokemonBattlePanel::AnimateHP(float MaxDuration) {
    auto CoreAttributes = CurrentBattler->GetAbilityComponent()->GetCoreAttributes();
    float HPPercent = HPBar->GetPercent();
    float OldHP = FMath::RoundToFloat(CoreAttributes->GetMaxHP() * HPPercent);
    float DrainRate =
        FMath::Min(FMath::Abs(OldHP - CoreAttributes->GetHP()) * UPokemonUIUtils::AnimationDrainSpeed, MaxDuration);
    float CurrentPercent = CurrentBattler->GetHPPercent();
    HPBarUpdateAnimation.PlayAnimation(HPPercent, CurrentPercent, DrainRate);
}

void UPokemonBattlePanel::RefreshStatusEffect() {
    // clang-format off
    auto Icon = CurrentBattler->GetStatusEffect() |
                Retro::Optionals::Transform<&FStatusEffectInfo::StatusEffectID>() |
                Retro::Optionals::AndThen([](FName ID) { return Pokemon::Assets::Graphics::StatusIcons.LoadAsset(ID); });
    // clang-format on
    if (!Icon.IsSet()) {
        StatusIcon->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    StatusIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    StatusIcon->SetBrushFromImageAsset(Icon.GetValue(), true);
}

void UPokemonBattlePanel::UpdateHPPercent(float NewPercent) {
    HPBar->SetPercent(NewPercent);
}

void UPokemonBattlePanel::HPPercentUpdateComplete() const {
    OnHPBarUpdated.Broadcast();
}