// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"

#include "SummaryHandler.generated.h"

class UPokemonSummaryScreen;
/**
 * The handler for showing a summary for a Pokémon
 */
UCLASS(EditInlineNew)
class POKEMONUI_API USummaryHandler : public UPartyMenuHandler {
    GENERATED_BODY()

  protected:
    void Handle_Implementation(const TScriptInterface<IPartyScreen> &Screen, const TScriptInterface<ITrainer> &Trainer,
                               int32 PartyIndex) override;

  private:
    UPROPERTY(EditAnywhere, Category = Screens)
    TSoftClassPtr<UPokemonSummaryScreen> SummaryScreenClass;
};
