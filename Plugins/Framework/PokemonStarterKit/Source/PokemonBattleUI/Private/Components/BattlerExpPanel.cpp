// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattlerExpPanel.h"
#include "Battle/Battlers/Battler.h"
#include "CommonNumericTextBlock.h"
#include "Components/ProgressBar.h"

/**
 * It should take about 0.25 s for the bar to fully fill up
 */
constexpr float AnimationGainSpeed = 1.75f;

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

UE5Coro::TCoroutine<> UBattlerExpPanel::AnimateGain(float MaxDuration) {
    using Pokemon::UI::FSetNewPercent;
    int32 LevelDiff = CurrentBattler->GetPokemonLevel() - DisplayedLevel;
    check(LevelDiff >= 0)
    float StartPercent = ExpBar->GetPercent();
    float EndPercent = CurrentBattler->GetExpPercent() + static_cast<float>(LevelDiff);

    float GainRate = FMath::Min((EndPercent - StartPercent) * AnimationGainSpeed, MaxDuration);
    co_await Pokemon::UI::ProgressBarAnimation(
        this, StartPercent, EndPercent, GainRate,
        FSetNewPercent::CreateUObject(this, &UBattlerExpPanel::UpdateExpBarPercent), true,
        FSimpleDelegate::CreateUObject(this, &UBattlerExpPanel::OnLevelUp));
    OnGainAnimationComplete.Broadcast();
}

void UBattlerExpPanel::BindOnAnimationComplete(FSimpleDelegate Callback) {
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