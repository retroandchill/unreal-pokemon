// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PartyMenuHandler.h"
#include "PartySwitchHandler.generated.h"

/**
 * The handler for switching two Pokémon in the party menu
 */
UCLASS(EditInlineNew)
class POKEMONUI_API UPartySwitchHandler : public UPartyMenuHandler {
	GENERATED_BODY()

public:
	bool ShouldShow(const IPartyScreen& Screen, const ITrainer& Trainer, int32 PartyIndex) const override;
	void Handle(IPartyScreen& Screen, const ITrainer& Trainer, int32 PartyIndex) override;
};
