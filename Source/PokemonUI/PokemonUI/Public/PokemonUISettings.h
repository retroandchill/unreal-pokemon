// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PokemonUISettings.generated.h"

class UTextDisplayScreen;
/**
 * Global configuration settings for the Pokémon UI classes
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Pokémon UI Settings"))
class POKEMONUI_API UPokemonUISettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	/**
	 * Get the name of the package that contains the Pokémon Icon graphics
	 * @return The name of the package that contains the Pokémon Icon graphics
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
	const FString& GetPokemonIconsPackageName() const;
	
	/**
	 * Get the base material used to construct Pokémon icons
	 * @return The base material used to construct Pokémon icons
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Icons")
	const FSoftObjectPath &GetPokemonIconsBaseMaterial() const;

	/**
	 * Get the name of the material property for the source texture of the icons
	 * @return The name of the material property for the source texture of the icons
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Icons")
	FName GetIconSourceTexturePropertyName() const;

	/**
	 * Get the name of the material property for the framerate of the icon animation
	 * @return The name of the material property for the framerate of the icon animation
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Icons")
	FName GetIconFrameRatePropertyName() const;

	/**
	 * Get the screen class use for the text screen
	 * @return The screen class use for the text screen
	 */
	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Menus")
	const TArray<TSubclassOf<UTextDisplayScreen>> &GetTextScreenClasses() const;

private:
	/**
	 * The name of the package that contains the Pokémon Icon graphics
	 */
	UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter=GetPokemonIconsPackageName, Category = "Search Paths")
	FString PokemonIconsPackageName;
	
	/**
	 * The base material used to construct Pokémon icons
	 */
	UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter=GetPokemonIconsBaseMaterial, Category = "Icons", meta = (AllowedClasses = "MaterialInterface"))
	FSoftObjectPath PokemonIconsBaseMaterial;

	/**
	 * The name of the material property for the source texture of the icons
	 */
	UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter=GetIconSourceTexturePropertyName, Category = "Icons")
	FName IconSourceTexturePropertyName = TEXT("SourceTexture");
	
	/**
	 * The name of the material property for the framerate of the icon animation
	 */
	UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter=GetIconFrameRatePropertyName, Category = "Icons")
	FName IconFrameRatePropertyName = TEXT("FrameRate");

	/**
	 * The screen class use for the text screen
	 */
	UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter=GetTextScreenClasses, Category = "Menus")
	TArray<TSubclassOf<UTextDisplayScreen>> TextScreenClasses;
};
