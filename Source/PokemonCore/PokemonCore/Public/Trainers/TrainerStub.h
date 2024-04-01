// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BasicTrainer.h"
#include "Trainer.h"

/**
 * Temporary stub class used to test trainer functionality before that component is fully built out.
 * TODO: Remove this class once the functionality is there.
 */
class POKEMONCORE_API FTrainerStub : public FBasicTrainer {
public:
	FTrainerStub();

	FTrainerStub(FTrainerStub&& Other) noexcept = default;
	FTrainerStub& operator=(FTrainerStub&& Other) noexcept = delete;

	TArray<TSharedRef<IPokemon>>& GetParty() override;
	virtual const TArray<TSharedRef<IPokemon>>& GetParty() const override;

private:
	TArray<TSharedRef<IPokemon>> Party;
};
