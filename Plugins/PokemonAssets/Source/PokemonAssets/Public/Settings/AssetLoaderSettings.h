// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "AssetLoaderSettings.generated.h"

class UTextureRepository;
class UStaticImageRepository;
/**
 * Global configuration settings for the Pokémon UI classes
 */
UCLASS(Config = Game, DefaultConfig)
class POKEMONASSETS_API UAssetLoaderSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Get the path to the repository data asset used to get the icons.
     * @return The path to the repository data asset used to get the icons.
     */
    UTextureRepository *GetPokemonSpriteRepository() const;

    /**
     * Get the path to the repository data asset used to get the trainers.
     * @return The path to the repository data asset used to get the trainers.
     */
    UTextureRepository *GetTrainerFrontSpriteRepository() const;

    /**
     * Get the path to the repository data asset used to get the types.
     * @return The path to the repository data asset used to get the types.
     */
    UStaticImageRepository *GetTypeIconRepository() const;

    /**
     * Get the path to the repository data asset used to get the statuses.
     * @return The path to the repository data asset used to get the statuses.
     */
    UStaticImageRepository *GetStatusIconRepository() const;

    /**
     * Get the path to the repository data asset used to get the Poké Ball icons on the summary screen.
     * @return The path to the repository data asset used to get the Poké Ball icons on the summary screen.
     */
    UStaticImageRepository *GetSummaryBallRepository() const;

    /**
     * Get the path to the repository data asset used to get the item icons used in the bag and summary screen.
     * @return The path to the repository data asset used to get the item icons used in the bag and summary screen.
     */
    UStaticImageRepository *GetItemIconRepository() const;

  private:
    /**
     * The path to the repository data asset used to get the icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders|Pokémon", meta = (AllowedClasses = "TextureRepository"))
    FSoftObjectPath PokemonSpriteRepository;

    /**
     * The path to the repository data asset used to get the trainers.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders|Trainers", meta = (AllowedClasses = "TextureRepository"))
    FSoftObjectPath TrainerFrontSpriteRepository;

    /**
     * The path to the repository data asset used to get the type icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders|UI", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath TypeIconRepository;

    /**
     * The path to the repository data asset used to get the status icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders|UI", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath StatusIconRepository;

    /**
     * The path to the repository data asset used to get the Poké Ball icons on the summary screen.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders|UI", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath SummaryBallRepository;

    /**
     * The path to the repository data asset used to get the item icons used in the bag and summary screen.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders|UI", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath ItemIconRepository;
};
