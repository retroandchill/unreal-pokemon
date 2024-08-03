﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattlerController.h"
#include "UObject/Object.h"

#include "AIBattlerController.generated.h"

class IBattler;

/**
 * The basic controller used by the AI to choose an action.
 */
UCLASS(meta = (DisplayName = "AI Battler Controller"))
class POKEMONBATTLE_API UAIBattlerController : public UObject, public IBattlerController {
    GENERATED_BODY()

  public:
    void InitiateActionSelection(const TScriptInterface<IBattler> &Battler) const override;
    void InitiateForcedSwitch(const TScriptInterface<IBattler> &Battler) const override;
    void BindOnActionReady(FActionReady &&QueueAction) override;

  private:
    /**
     * Perform an asynchronous action selection process
     */
    void ChooseAction(TScriptInterface<IBattler> Battler) const;

    void ChoosePokemonToSwitchTo(TScriptInterface<IBattler> Battler) const;

    /**
     * Called when an action is ready to be executed
     */
    FActionReady ActionReady;
};
