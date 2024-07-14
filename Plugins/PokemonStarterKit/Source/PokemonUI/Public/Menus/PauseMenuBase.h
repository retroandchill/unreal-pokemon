// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonMenuBase.h"
#include "PauseMenuBase.generated.h"

class IPokemon;
class UPokemonInfoButtonBase;
/**
 * The base class for the pause menu in the game.
 */
UCLASS(Abstract)
class POKEMONUI_API UPauseMenuBase : public UPokemonMenuBase {
    GENERATED_BODY()

};
