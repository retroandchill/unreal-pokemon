// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattlerExpPanel.h"
#include "CommonNumericTextBlock.h"
#include "Battle/Battlers/Battler.h"
#include "Components/DisplayText.h"
#include "Components/ProgressBar.h"

/**
 * It should take about 0.25 s for the bar to fully fill up
 */
constexpr float AnimationGainSpeed = 1.75f;

using FUpdatePercent = Pokemon::UI::FSetNewPercent::FDelegate;
using FUpdateComplete = Pokemon::UI::FOnAnimationComplete::FDelegate;

void UBattlerExpPanel::NativeConstruct() {
    Super::NativeConstruct();
    ExpBarAnimation.BindActionToPercentDelegate(
        FUpdatePercent::CreateUObject(this, &UBattlerExpPanel::UpdateExpBarPercent));
    ExpBarAnimation.BindActionToWrapAroundAnimation(FUpdateComplete::CreateUObject(this, &UBattlerExpPanel::OnLevelUp));
    ExpBarAnimation.BindActionToCompleteDelegate(
        FUpdateComplete::CreateUObject(this, &UBattlerExpPanel::OnExpGainComplete));
}

void UBattlerExpPanel::SetBattler(const TScriptInterface<IBattler> &Battler, const TOptional<int32> &Level,
                                  const TOptional<float> &ExpGainPercent) {
    CurrentBattler = Battler;
    DisplayedLevel = Level.Get(Battler->GetPokemonLevel());
    LevelText->SetText(FText::FromString(FString::FromInt(DisplayedLevel)));
    ExpBar->SetPercent(ExpGainPercent.Get(Battler->GetExpPercent()));
    ChangeExpGainDisplay(0);
    OnBattlerSet(CurrentBattler);
}

void UBattlerExpPanel::ChangeExpGainDisplay(int32 Gain) {
    static const auto GainFormat = FText::FromStringView(TEXT("+{0}"));
    ExpGainText->SetText(FText::Format(GainFormat, {Gain}));
    ExpGain = Gain;
}

void UBattlerExpPanel::AnimateGain(float MaxDuration) {
    int32 LevelDiff = CurrentBattler->GetPokemonLevel() - DisplayedLevel;
    check(LevelDiff >= 0)
    float StartPercent = ExpBar->GetPercent();
    float EndPercent = CurrentBattler->GetExpPercent() + static_cast<float>(LevelDiff);

    float GainRate = FMath::Min((EndPercent - StartPercent) * AnimationGainSpeed, MaxDuration);
    ExpBarAnimation.PlayAnimation(StartPercent, EndPercent, GainRate, true);
}

void UBattlerExpPanel::BindOnAnimationComplete(FSimpleDelegate &&Callback) {
    OnGainAnimationComplete.Add(std::move(Callback));
}

void UBattlerExpPanel::UpdateExpBarPercent(float NewPercent) {
    ExpBar->SetPercent(NewPercent);
}

void UBattlerExpPanel::OnLevelUp() {
    DisplayedLevel++;
    LevelText->SetCurrentValue(static_cast<float>(DisplayedLevel));
    DisplayLevelUp();
}

void UBattlerExpPanel::OnExpGainComplete() const {
    OnGainAnimationComplete.Broadcast();
}