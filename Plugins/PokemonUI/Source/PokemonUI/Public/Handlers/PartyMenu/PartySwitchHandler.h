// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"

#include "PartySwitchHandler.generated.h"

/**
 * The handler for switching two Pokémon in the party menu
 */
UCLASS(EditInlineNew)
class POKEMONUI_API UPartySwitchHandler : public UPartyMenuHandler {
    GENERATED_BODY()

  protected:
    bool ShouldShow_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                   const TScriptInterface<ITrainer> &Trainer, int32 PartyIndex) const override;
    void Handle_Implementation(const TScriptInterface<IPartyScreen> &Screen, const TScriptInterface<ITrainer> &Trainer,
                               int32 PartyIndex) override;
};
