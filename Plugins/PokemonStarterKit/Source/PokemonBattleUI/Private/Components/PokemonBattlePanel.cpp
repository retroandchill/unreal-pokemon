// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonBattlePanel.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Utilities/PokemonUIUtils.h"
#include "Utilities/WidgetUtilities.h"

using FUpdatePercent = Pokemon::UI::FSetNewPercent::FDelegate;
using FUpdateComplete = Pokemon::UI::FOnAnimationComplete::FDelegate;

/**
 * The default animation drain speed in generation 4. This is 1 HP every 2 frames of a 60 FPS drain.
 */
constexpr float AnimationDrainSpeed = 2.f / 60.f;

void UPokemonBattlePanel::NativeConstruct() {
    Super::NativeConstruct();
    HPBarUpdateAnimation.BindActionToPercentDelegate(
        FUpdatePercent::CreateUObject(this, &UPokemonBattlePanel::UpdateHPPercent));
    HPBarUpdateAnimation.BindActionToCompleteDelegate(
        FUpdateComplete::CreateUObject(this, &UPokemonBattlePanel::HPPercentUpdateComplete));
}

const TScriptInterface<IBattler> &UPokemonBattlePanel::GetCurrentBattler() const {
    return CurrentBattler;
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

    PokemonName->SetText(CurrentBattler->GetNickname());
    PokemonLevel->SetText(FText::FromString(FString::FromInt(CurrentBattler->GetPokemonLevel())));
    HPBar->SetPercent(CurrentBattler->GetHPPercent());

    auto Gender = CurrentBattler->GetGender();
    UPokemonUIUtils::SetPokemonGenderText(Gender, PokemonGender);
    if (GenderTextColors.Contains(Gender)) {
        PokemonGender->SetTextStyle(GenderTextColors[Gender]);
    }

    RefreshStatusEffect();
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
    float DrainRate = FMath::Min(FMath::Abs(OldHP - CoreAttributes->GetHP()) * AnimationDrainSpeed, MaxDuration);
    float CurrentPercent = CurrentBattler->GetHPPercent();
    HPBarUpdateAnimation.PlayAnimation(HPPercent, CurrentPercent, DrainRate);
}

void UPokemonBattlePanel::RefreshStatusEffect() {
    if (auto &Status = CurrentBattler->GetStatusEffect(); Status.IsSet()) {
        auto GraphicsSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
        auto Icon = GraphicsSubsystem->GetStatusIconGraphic(Status.GetValue().StatusEffectID);
        StatusIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        UWidgetUtilities::SetBrushFromAsset(StatusIcon, Icon, true);
    } else {
        StatusIcon->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UPokemonBattlePanel::UpdateHPPercent(float NewPercent) {
    HPBar->SetPercent(NewPercent);
}

void UPokemonBattlePanel::HPPercentUpdateComplete() const {
    OnHPBarUpdated.Broadcast();
}