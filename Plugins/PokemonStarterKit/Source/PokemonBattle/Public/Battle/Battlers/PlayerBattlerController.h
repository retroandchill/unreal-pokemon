// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattlerController.h"
#include "UObject/Object.h"

#include "PlayerBattlerController.generated.h"

class IBattle;
/**
 * The battler controller for the player
 */
UCLASS()
class POKEMONBATTLE_API UPlayerBattlerController : public UObject, public IBattlerController {
    GENERATED_BODY()

  public:
    /**
     * Set the battle that this controller is a part of
     * @param Battle The battle this controller is a part of, that is used to prompt the HUD
     */
    UPlayerBattlerController *SetBattle(const TScriptInterface<IBattle> &Battle);

    void InitiateActionSelection(const TScriptInterface<IBattler> &Battler) const override;
    void InitiateForcedSwitch(const TScriptInterface<IBattler>& Battler) const override;
    void BindOnActionReady(FActionReady &&QueueAction) override;

  private:
    /**
     * The battle this controller is a part of, that is used to prompt the HUD
     */
    UPROPERTY()
    TScriptInterface<IBattle> BattleContext;

    /**
     * Called when an action is ready to be executed
     */
    FActionReady ActionReady;
};
