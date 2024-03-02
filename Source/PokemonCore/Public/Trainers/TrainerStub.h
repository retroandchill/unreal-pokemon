// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Trainer.h"

/**
 * Temporary stub class used to test trainer functionality before that component is fully built out.
 * TODO: Remove this class once the functionality is there.
 */
class POKEMONCORE_API FTrainerStub : public ITrainer {
public:
	FTrainerStub();

	const TArray<TSharedRef<IPokemon>>& GetParty() override;
	const TArray<TSharedRef<const IPokemon>>& GetParty() const override;

private:
	TArray<TSharedRef<IPokemon>> Party;
};
