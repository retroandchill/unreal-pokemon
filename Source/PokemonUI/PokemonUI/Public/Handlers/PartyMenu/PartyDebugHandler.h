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

public:
	bool ShouldShow(const IPartyScreen& Screen, const TArray<TScriptInterface<IPokemon>>& Party, int32 PartyIndex) const override;
	void Handle(IPartyScreen& Screen, TArray<TScriptInterface<IPokemon>>& Party, int32 PartyIndex) override;
};
