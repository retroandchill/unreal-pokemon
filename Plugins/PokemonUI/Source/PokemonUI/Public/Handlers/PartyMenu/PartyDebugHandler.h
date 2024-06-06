// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"

#include "PartyDebugHandler.generated.h"

/**
 * Handler for showing the debug options for the Pokémon. Only appears during Play-in-Editor mode.
 */
UCLASS()
class POKEMONUI_API UPartyDebugHandler : public UPartyMenuHandler {
    GENERATED_BODY()

  protected:
    bool ShouldShow_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) const override;
    void Handle_Implementation(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) override;
};
