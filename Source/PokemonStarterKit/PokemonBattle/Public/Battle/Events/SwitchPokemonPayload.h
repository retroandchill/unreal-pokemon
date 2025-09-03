// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "SwitchPokemonPayload.generated.h"

class ITrainer;
/**
 * The payload sent when switching Pokémon
 */
UCLASS()
class POKEMONBATTLE_API USwitchPokemonPayload : public UObject {
    GENERATED_BODY()

  public:
    /**
     * The unique ID of the owning trainer.
     */
    TScriptInterface<ITrainer> OwningTrainer;

    /**
     * The index in the owning trainer's party to swap from
     */
    int32 UserIndex = 0;

    /**
     * The index in the owning trainer's party to swap to
     */
    int32 SwapIndex = 0;
};
