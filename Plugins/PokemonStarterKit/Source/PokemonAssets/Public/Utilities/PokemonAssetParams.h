// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "UObject/Object.h"
#include "PokemonAssetParams.generated.h"

/**
 * @struct FPokemonAssetParams
 * @brief Contains parameters used to define specific properties for Pokémon assets, such as Form, Gender, Shiny status, and Shadow status.
 *
 * This structure is utilized to customize and resolve Pokémon asset paths based on specified parameters. It supports specifying the form, gender, shiny, and shadow variations of a Pokémon.
 *
 * The structure can be exposed to Blueprints, allowing game designers to configure these parameters directly within the Unreal Engine editor. The UPROPERTY macros facilitate this integration.
 *
 * @param Form Specifies the form of the Pokémon. This influences the resolved asset path if a specific form is provided.
 * @param Gender Specifies the gender of the Pokémon. This affects the resolved asset path based on gender variations.
 * @param bShiny Determines whether the Pokémon is shiny. Shiny status influences the resolved asset path to include shiny-specific assets.
 * @param bShadow Determines whether the Pokémon is shadow. Shadow status influences the resolved asset path to include shadow-specific assets.
 *
 * @fn CreatePokemonSpriteResolutionList
 * Generates a list of sprite resolutions based on the Pokémon species and specified subfolder. Various factors such as shiny, shadow, gender, and form are considered while creating this list.
 *
 * @param Species The species of the Pokémon. This is the primary identifier used to generate the sprite resolution list.
 * @param Subfolder Specifies the subfolder within the asset directory structure where the sprites are stored.
 *
 * @return TArray<FString> A list of formatted sprite resolution strings based on the provided parameters and subfolder.
 */
USTRUCT(BlueprintType)
struct POKEMONASSETS_API FPokemonAssetParams {
    GENERATED_BODY()

    /**
     * Specifies the form of the Pokémon. This influences the resolved asset path if a specific form is provided.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites, meta = (UIMin = 0, ClampMin = 0))
    FName Form = NAME_None;

    /**
     * Specifies the gender of the Pokémon. This affects the resolved asset path based on gender variations.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    EPokemonGender Gender = EPokemonGender::Male;

    /**
     * Determines whether the Pokémon is shiny. Shiny status influences the resolved asset path to include shiny-specific assets.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    bool bShiny = false;

    /**
     * Determines whether the Pokémon is shadow. Shadow status influences the resolved asset path to include shadow-specific assets.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    bool bShadow = false;

    /**
     * Generates a list of sprite resolutions based on the Pokémon species and specified subfolder. Various factors such as shiny, shadow, gender, and form are considered while creating this list.
     * @param Species The species of the Pokémon. This is the primary identifier used to generate the sprite resolution list.
     * @param Subfolder Specifies the subfolder within the asset directory structure where the sprites are stored.
     * @return A list of formatted sprite resolution strings based on the provided parameters and subfolder.
     */
    TArray<FString> CreateResolutionList(FName Species, FStringView Subfolder = FStringView()) const;
};
