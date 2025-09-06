// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"

#include "ExpGainPane.generated.h"

struct FExpGainInfo;
class IBattler;
class IBattle;
class UBattlerExpPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExpGainComplete);

/**
 *
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UExpGainPane : public UCommonUserWidget
{
    GENERATED_BODY()

  public:
    void SetBattle(const TScriptInterface<IBattle> &Battle);

    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SlotBattlerPanel(UBattlerExpPanel *Panel);

    void GainExp(TArray<FExpGainInfo> &&GainInfosIn);

    UFUNCTION(BlueprintCallable, Category = Exp)
    void PlayExpGain(float MaxDuration = 3.f);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Exp)
    const TArray<FExpGainInfo> &GetGainInfos() const;

  private:
    UBattlerExpPanel *CreateBattlerPanel(const TScriptInterface<IBattler> &Battler);

    UPROPERTY()
    TScriptInterface<IBattle> OwningBattle;

    UPROPERTY()
    TArray<TObjectPtr<UBattlerExpPanel>> Panels;

    UPROPERTY(EditAnywhere, Category = Content)
    TSubclassOf<UBattlerExpPanel> PanelClass;

    int32 AnimationsComplete = 0;

    UPROPERTY(BlueprintAssignable, Category = Events)
    FOnExpGainComplete OnExpGainComplete;

    UPROPERTY(BlueprintGetter = GetGainInfos, Category = Exp)
    TArray<FExpGainInfo> GainInfos;
};
