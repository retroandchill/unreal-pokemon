﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonUISettings.generated.h"

class UTextDisplayScreen;
/**
 * Global configuration settings for the Pokémon UI classes
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Pokémon UI Settings"))
class POKEMONUI_API UPokemonUISettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Get the name of the package that contains the Pokémon Icon graphics
     * @return The name of the package that contains the Pokémon Icon graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetPokemonIconsPackageName() const;

    /**
     * Get the name of the package that contains the Trainer Sprite graphics
     * @return The name of the package that contains the Trainer Sprite graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetTrainerSpritesPackageName() const;

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
     * Get the base material used to construct Trainer sprites in the UI
     * @return The base material used to construct Trainer sprites in the UI
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Trainers")
    const FSoftObjectPath &GetTrainerSpriteBaseMaterial() const;

    /**
     * Get the name of the material property for the source texture of the trainer sprites
     * @return The name of the material property for the source texture of the trainer sprites
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Trainers")
    FName GetTrainerSpriteSourceTexturePropertyName() const;

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
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetPokemonIconsPackageName, Category = "Search Paths")
    FString PokemonIconsPackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTrainerSpritesPackageName, Category = "Search Paths")
    FString TrainerSpritesPackageName;

    /**
     * The base material used to construct Pokémon icons
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetPokemonIconsBaseMaterial, Category = "Pokémon|Icons",
              meta = (AllowedClasses = "MaterialInterface"))
    FSoftObjectPath PokemonIconsBaseMaterial;

    /**
     * The name of the material property for the source texture of the icons
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetIconSourceTexturePropertyName, Category = "Pokémon|Icons")
    FName IconSourceTexturePropertyName = TEXT("SourceTexture");

    /**
     * The name of the material property for the framerate of the icon animation
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetIconFrameRatePropertyName, Category = "Pokémon|Icons")
    FName IconFrameRatePropertyName = TEXT("FrameRate");

    /**
     * The base material used to construct Trainer sprites in the UI
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTrainerSpriteBaseMaterial,
              DisplayName = "Trainer Sprite Base Material (UI)", Category = "Trainers",
              meta = (AllowedClasses = "MaterialInterface"))
    FSoftObjectPath TrainerSpriteBaseMaterial;

    /**
     * The name of the material property for the source texture of the trainer sprites
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTrainerSpriteSourceTexturePropertyName,
              Category = "Trainers")
    FName TrainerSpriteSourceTexturePropertyName = TEXT("SourceTexture");

    /**
     * The screen class use for the text screen
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTextScreenClasses, Category = "Menus")
    TArray<TSubclassOf<UTextDisplayScreen>> TextScreenClasses;
};
