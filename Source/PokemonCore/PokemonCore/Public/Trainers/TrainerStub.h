// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BasicTrainer.h"
#include "Trainer.h"
#include "TrainerStub.generated.h"

/**
 * Temporary stub class used to test trainer functionality before that component is fully built out.
 * TODO: Remove this class once the functionality is there.
 */
UCLASS()
class POKEMONCORE_API UTrainerStub : public UBasicTrainer {
	GENERATED_BODY()
	
public:
	TScriptInterface<ITrainer> Initialize(FName NewTrainerType, FText NewTrainerName) override;
	
};
