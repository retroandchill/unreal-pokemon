// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ExpGainPane.generated.h"

class IBattler;
class IBattle;
class UBattlerExpPanel;
/**
 * 
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UExpGainPane : public UCommonUserWidget {
    GENERATED_BODY()

public:
    void SetBattle(const TScriptInterface<IBattle>& Battle);

    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SlotBattlerPanel(UBattlerExpPanel* Panel);
    
private:
    UBattlerExpPanel* CreateBattlerPanel(const TScriptInterface<IBattler>& Battler);
    
    UPROPERTY()
    TScriptInterface<IBattle> OwningBattle;
    
    UPROPERTY()
    TArray<TObjectPtr<UBattlerExpPanel>> Panels;

    UPROPERTY(EditAnywhere, Category = Content)
    TSubclassOf<UBattlerExpPanel> PanelClass;

};
