// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"
#include "SummaryHandler.generated.h"

/**
 * The handler for showing a summary for a Pokémon
 */
UCLASS(EditInlineNew)
class POKEMONUI_API USummaryHandler : public UPartyMenuHandler {
	GENERATED_BODY()

public:
	void Handle(IPartyScreen& Screen, const ITrainer& Trainer, int32 PartyIndex) override;
};
