// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GraphicsLoadingSubsystem.generated.h"

class IPokemon;
/**
 * Subsystem designed to handle the loading of graphical assets into memory
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UGraphicsLoadingSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	/**
	 * Get the icon based upon the Pokémon that was passed in.
	 * @param Pokemon The Pokémon to get the icon for
	 * @return The graphical asset that this icon refers to.
	 */
	UObject *GetPokemonIcon(const IPokemon &Pokemon);

	/**
	 * Get the icon based upon the Pokémon information that was passed in.
	 * @param Species The species of Pokémon to get the icon for
	 * @return The graphical asset that this icon refers to.
	 */
	UObject *GetPokemonIcon(FName Species);
	
private:
	/**
	 * The name of the package that contains the Pokémon Icon graphics
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Search Paths")
	FString PokemonIconsPackageName;
};
