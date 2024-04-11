// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DefaultMainBattleStatEntry.generated.h"
#include "DefaultStatEntry.h"

/**
 * Handles the default representation of the main battle stats
 */
UCLASS()
class POKEMONCORE_API UDefaultMainBattleStatEntry : public UDefaultStatEntry {
    GENERATED_BODY()

  public:
    void RefreshValue(int32 Level, int32 Base, const FNature &Nature) final;
};
