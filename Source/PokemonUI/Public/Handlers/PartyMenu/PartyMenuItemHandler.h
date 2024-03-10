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

public:
	bool ShouldShow(const IPartyScreen& Screen, const TArray<TSharedRef<IPokemon>>& Party, int32 PartyIndex) const override;
	void Handle(IPartyScreen& Screen, TArray<TSharedRef<IPokemon>>& Party, int32 PartyIndex) override;
};
