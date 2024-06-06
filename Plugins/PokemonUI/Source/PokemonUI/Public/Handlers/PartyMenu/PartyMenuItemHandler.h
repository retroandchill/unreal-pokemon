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

private:
    /**
     * The text the help window is changed to during this handler's execution.
     */
    UPROPERTY(EditAnywhere, Category = Prompt)
    FText HelpText;

    /**
     * The handlers for the sub-commands for items
     */
    UPROPERTY(EditAnywhere, Category = Prompt)
    TArray<TObjectPtr<UPartyMenuHandler>> SubCommands;
};
