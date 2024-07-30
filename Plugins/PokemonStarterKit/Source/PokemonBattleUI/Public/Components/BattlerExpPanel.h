// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
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

public:
    void SetBattler(const TScriptInterface<IBattler>& Battler);

    void ChangeExpGainDisplay(int32 Gain);
    
protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void OnBattlerSet(const TScriptInterface<IBattler>& Battler);

private:
    UPROPERTY()
    TScriptInterface<IBattler> CurrentBattler;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> LevelText;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ExpBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> ExpGainText;

};
