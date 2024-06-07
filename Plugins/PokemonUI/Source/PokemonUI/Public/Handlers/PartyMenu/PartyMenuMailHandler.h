// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"

#include "PartyMenuMailHandler.generated.h"

/**
 * Handler for the mail option for a Pokémon
 */
UCLASS()
class POKEMONUI_API UPartyMenuMailHandler : public UPartyMenuHandler {
    GENERATED_BODY()

  protected:
    bool ShouldShow_Implementation(const TScriptInterface<IPartyScreen> &Screen,
                                   const TScriptInterface<ITrainer> &Trainer, int32 PartyIndex) const override;
    void Handle_Implementation(const TScriptInterface<IPartyScreen> &Screen, const TScriptInterface<ITrainer> &Trainer,
                               int32 PartyIndex) override;
};
