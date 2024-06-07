// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "AssetAssignmentSettings.generated.h"

/**
 * Settings related to the assignment of assets into the asset loaders
 */
UCLASS(Config = Editor, DefaultConfig)
class POKEMONEDITORTOOLS_API UAssetAssignmentSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Get the name of the package that contains the Pokémon Icon graphics
     * @return The name of the package that contains the Pokémon Icon graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetPokemonSpritePackageName() const;

    /**
     * Get the name of the package that contains the Trainer Sprite graphics
     * @return The name of the package that contains the Trainer Sprite graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetTrainerSpritesPackageName() const;

    /**
     * Get the name of the package that contains the Type graphics
     * @return The name of the package that contains the Type graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetTypeIconsPackageName() const;

    /**
     * Get the name of the package that contains the Status graphics
     * @return The name of the package that contains the Status graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetStatusIconsPackageName() const;

    /**
     * Get the name of the package that contains the summary screen Poké Ball graphics
     * @return The name of the package that contains the summary screen Poké Ball graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetSummaryBallPackageName() const;

    /**
     * Get the name of the package that contains the item icon graphics
     * @return The name of the package that contains the item icon graphics
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Search Paths")
    const FString &GetItemIconPackageName() const;

  private:
    /**
     * The name of the package that contains the Pokémon Icon graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetPokemonSpritePackageName, Category = "Search Paths")
    FString PokemonSpritePackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTrainerSpritesPackageName, Category = "Search Paths")
    FString TrainerSpritesPackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTypeIconsPackageName, Category = "Search Paths")
    FString TypeIconsPackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetStatusIconsPackageName, Category = "Search Paths")
    FString StatusIconsPackageName;

    /**
     * The name of the package that contains the summary screen Poké Ball graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetSummaryBallPackageName, Category = "Search Paths")
    FString SummaryBallPackageName;

    /**
     * The name of the package that contains the item icon graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetItemIconPackageName, Category = "Search Paths")
    FString ItemIconPackageName;
};
