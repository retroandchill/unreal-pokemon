// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Animations/ProgressBarAnimation.h"

#include "BattlerExpPanel.generated.h"

class IBattler;
class UDisplayText;
class UProgressBar;



/**
 * 
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattlerExpPanel : public UCommonUserWidget {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;
    
public:
    void SetBattler(const TScriptInterface<IBattler>& Battler, const TOptional<int32>& Level = TOptional<int32>(), const TOptional<float> &ExpGainPercent = TOptional<float>());

    void ChangeExpGainDisplay(int32 Gain);
    void AnimateGain(float MaxDuration = 3.f);

    void BindOnAnimationComplete(FSimpleDelegate&& Callback);
    
protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void OnBattlerSet(const TScriptInterface<IBattler>& Battler);

    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void DisplayLevelUp();

private:
    void UpdateExpBarPercent(float NewPercent);
    void OnLevelUp();
    void OnExpGainComplete() const;
    
    UPROPERTY()
    TScriptInterface<IBattler> CurrentBattler;

    int32 DisplayedLevel = 0;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> LevelText;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ExpBar;

    int32 ExpGain = 0;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> ExpGainText;

    Pokemon::UI::FProgressBarAnimation ExpBarAnimation;

    FSimpleMulticastDelegate OnGainAnimationComplete;

};
