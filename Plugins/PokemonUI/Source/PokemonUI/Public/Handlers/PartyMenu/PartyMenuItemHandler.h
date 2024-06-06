// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"

#include "PartyMenuItemHandler.generated.h"

/**
 * Handling the item controls of the party menu
 */
UCLASS()
class POKEMONUI_API UPartyMenuItemHandler : public UPartyMenuHandler {
    GENERATED_BODY()

  protected:
    bool ShouldShow_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) const override;
    void Handle_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) override;
};
