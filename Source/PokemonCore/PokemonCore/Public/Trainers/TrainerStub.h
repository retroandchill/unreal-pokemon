// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Trainer.h"
#include "TrainerStub.generated.h"

/**
 * Temporary stub class used to test trainer functionality before that component is fully built out.
 * TODO: Remove this class once the functionality is there.
 */
UCLASS()
class POKEMONCORE_API UTrainerStub : public UObject, public ITrainer {
public:
	void Initialize();
	
	TArray<TScriptInterface<IPokemon>>& GetParty() override;
	virtual const TArray<TScriptInterface<IPokemon>>& GetParty() const override;

private:
	UPROPERTY()
	TArray<TScriptInterface<IPokemon>> Party;
};
