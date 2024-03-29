// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GraphicsLoadingSubsystem.generated.h"

class IPokemon;
/**
 * Subsystem designed to handle the loading of graphical assets into memory
 */
UCLASS()
class POKEMONUI_API UGraphicsLoadingSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Get the icon based upon the Pokémon that was passed in.
	 * @param Pokemon The Pokémon to get the icon for
	 * @param Outer The owner of the created material instance
	 * @return The graphical asset that this icon refers to.
	 */
	UMaterialInstanceDynamic* GetPokemonIcon(const IPokemon& Pokemon, UObject* Outer);

	/**
	 * Get the icon based upon the Pokémon information that was passed in.
	 * @param Species The species of Pokémon to get the icon for
	 * @param Outer The owner of the created material instance
	 * @return The graphical asset that this icon refers to.
	 */
	UMaterialInstanceDynamic* GetPokemonIcon(FName Species, UObject* Outer);

private:
	/**
	 * The name of the package that contains the Pokémon Icon graphics
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Search Paths")
	FString PokemonIconsPackageName;

	/**
	 * The base material used to construct Pokémon icons
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Icons")
	TObjectPtr<UMaterialInterface> PokemonIconsBaseMaterial;

	/**
	 * The name of the material property for the source texture of the icons
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Icons")
	FName IconSourceTexturePropertyName;
	
	/**
	 * The name of the material property for the framerate of the icon animation
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Icons")
	FName IconFrameRatePropertyName;
};
