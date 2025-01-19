// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UE5Coro.h"
#include "UObject/Interface.h"

#include "BattleHUD.generated.h"

struct FExpGainInfo;
class IBattler;
class IBattleAction;
// This class does not need to be modified.
UINTERFACE()
class POKEMONBATTLE_API UBattleHUD : public UInterface {
    GENERATED_BODY()
};

/**
 *
 */
class POKEMONBATTLE_API IBattleHUD {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual void Refresh() const = 0;
    
    virtual void SelectAction(const TScriptInterface<IBattler> &Battler) = 0;

    virtual void ClearSelectingBattlers() = 0;

    virtual UE5Coro::TCoroutine<> DisplayExpForGain(UE5Coro::TLatentContext<const UObject> Context, TArray<FExpGainInfo> GainInfos) = 0;

    virtual UE5Coro::TCoroutine<> AnimateHPChange(const TScriptInterface<IBattler> &Battler, float MaxDuration = 1.f, FForceLatentCoroutine = {}) = 0;

    virtual void PromptMandatorySwitch(const TScriptInterface<IBattler>& Battler) = 0;
};