// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Data/CommandCondition.h"
#include "HasPokemonInParty.generated.h"

/**
 * Condition to check if the player has Pokémon in their party?
 */
UCLASS(Blueprintable, EditInlineNew)
class POKEMONUI_API UHasPokemonInParty : public UCommandCondition {
    GENERATED_BODY()

protected:
    bool IsEnabled_Implementation() const override;

};
