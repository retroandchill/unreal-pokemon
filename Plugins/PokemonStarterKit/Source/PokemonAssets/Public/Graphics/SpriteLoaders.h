#pragma once

#include "CoreMinimal.h"

#include "SpriteLoaders.generated.h"

struct FSpriteRepositories;
class UStaticImageRepository;
class UTextureRepository;
/**
 * Container for the actual sprite data used by the graphics loading subsystem
 */
USTRUCT(BlueprintType)
struct POKEMONASSETS_API FSpriteLoaders {
GENERATED_BODY()

    /**
     * The path to the repository data asset used to get the icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders")
    TSoftObjectPtr<UTextureRepository> PokemonSpriteRepository;

    /**
     * The path to the repository data asset used to get the trainers.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders")
    TSoftObjectPtr<UTextureRepository> TrainerFrontSpriteRepository;

    /**
     * The path to the repository data asset used to get the type icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders")
    TSoftObjectPtr<UStaticImageRepository> TypeIconRepository;

    /**
     * The path to the repository data asset used to get the status icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders")
    TSoftObjectPtr<UStaticImageRepository> StatusIconRepository;

    /**
     * The path to the repository data asset used to get the Poké Ball icons on the summary screen.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders")
    TSoftObjectPtr<UStaticImageRepository> SummaryBallRepository;

    /**
     * The path to the repository data asset used to get the item icons used in the bag and summary screen.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders")
    TSoftObjectPtr<UStaticImageRepository> ItemIconRepository;

    /**
     * Construct the default version of the struct.
     */
    FSpriteLoaders();

    /**
     * Construct a new instance of this object from the supplied settings.
     * @param Settings The settings to copy the information from
     */
    explicit FSpriteLoaders(const FSpriteRepositories &Settings);

    /**
     * Copy over the reference form the supplied settings and assign them into this object
     * @param Settings The settings to copy the information from
     * @return A reference to the target object
     */
    FSpriteLoaders &operator=(const FSpriteRepositories &Settings);
    
};
