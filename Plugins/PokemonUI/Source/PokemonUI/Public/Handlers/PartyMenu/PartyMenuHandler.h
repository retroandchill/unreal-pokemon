// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Handlers/MenuHandler.h"
#include "UObject/Object.h"

#include "PartyMenuHandler.generated.h"

class IPartyScreen;
class IPokemon;
class ITrainer;

/**
 * Basic abstract handlers for the party menu
 */
UCLASS(Blueprintable, Abstract, EditInlineNew)
class POKEMONUI_API UPartyMenuHandler : public UMenuHandler {
    GENERATED_BODY()

  public:
    /**
     * Should this handler show the underlying command to the player?
     * @param Screen The screen currently being shown to the player
     * @param Trainer The current player
     * @param PartyIndex The index of the party that is currently selected
     * @return Should the command be shown?
     */
    UFUNCTION(BlueprintNativeEvent, Category = Display)
    bool ShouldShow(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex) const;

    /**
     * Handle the effect of the command
     * @param Screen The screen currently being shown to the player
     * @param Trainer The current player
     * @param PartyIndex The index of the party that is currently selected
     */
    UFUNCTION(BlueprintNativeEvent, Category = Action)
    void Handle(const TScriptInterface<IPartyScreen>& Screen, const TScriptInterface<ITrainer>& Trainer, int32 PartyIndex);
};
