// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DefaultStatEntry.h"
#include "DefaultMainStatEntry.generated.h"

/**
 * Represents the calculation implementation of the default main stat
 */
UCLASS()
class POKEMONCORE_API UDefaultMainStatEntry : public UDefaultStatEntry {
	GENERATED_BODY()
	
public:
	void RefreshValue(int32 Level, int32 Base, const FNature& Nature) final;
	
};
