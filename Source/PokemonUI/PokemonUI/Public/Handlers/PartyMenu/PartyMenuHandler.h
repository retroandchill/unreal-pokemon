// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PartyMenuHandler.generated.h"

class IPartyScreen;
class IPokemon;
class ITrainer;

/**
 * Basic abstract handlers for the party menu
 */
UCLASS(Blueprintable, Abstract, EditInlineNew)
class POKEMONUI_API UPartyMenuHandler : public UObject {
	GENERATED_BODY()

public:
	/**
	 * Get the ID of the command
	 * @return The ID of the command
	 */
	FName GetID() const;

	/**
	 * Get the text displayed to the player
	 * @return The text displayed to the player
	 */
	const FText &GetText() const;

	/**
	 * Should this handler show the underlying command to the player?
	 * @param Screen The screen currently being shown to the player
	 * @param Trainer The current player
	 * @param PartyIndex The index of the party that is currently selected
	 * @return Should the command be shown?
	 */
	virtual bool ShouldShow(const IPartyScreen& Screen, const ITrainer& Trainer, int32 PartyIndex) const;

	/**
	 * Handle the effect of the command
	 * @param Screen The screen currently being shown to the player
	 * @param Trainer The current player
	 * @param PartyIndex The index of the party that is currently selected
	 */
	virtual void Handle(IPartyScreen& Screen, const ITrainer& Trainer, int32 PartyIndex);
	
private:
	/**
	 * The ID of the command
	 */
	UPROPERTY(EditAnywhere, Category = "Menu Handlers")
	FName ID;

	/**
	 * The text displayed to the player
	 */
	UPROPERTY(EditAnywhere, Category = "Menu Handlers")
	FText Text;
};
