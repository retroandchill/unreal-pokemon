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

public:
	bool ShouldShow(const IPartyScreen& Screen, const TArray<TSharedRef<IPokemon>>& Party, int32 PartyIndex) const override;
	void Handle(IPartyScreen& Screen, TArray<TSharedRef<IPokemon>>& Party, int32 PartyIndex) override;
};
