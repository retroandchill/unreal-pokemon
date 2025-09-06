// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "BattlerSwitchPanel.generated.h"

class IBattler;

/**
 * Selectable panel used for switching out Pokémon in battle.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattlerSwitchPanel : public UCommonButtonBase
{
    GENERATED_BODY()

  public:
    /**
     * Get the displayed battler
     * @return The battler that is being displayed in this panel
     */
    UFUNCTION(BlueprintPure, Category = Content)
    const TScriptInterface<IBattler> &GetBattler() const;

    /**
     * Set the displayed battler
     * @param Battler The battler that will be displayed in this panel
     */
    void SetBattler(const TScriptInterface<IBattler> &Battler);

    /**
     * Update the visuals for this particular panel
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void Refresh();

  private:
    /**
     * The battler that is being displayed in this panel
     */
    UPROPERTY(BlueprintGetter = GetBattler, Category = Content)
    TScriptInterface<IBattler> CurrentBattler;
};
