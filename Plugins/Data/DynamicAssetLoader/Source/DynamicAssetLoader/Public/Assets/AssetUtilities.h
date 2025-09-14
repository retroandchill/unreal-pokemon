// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AssetUtilities.generated.h"

/**
 * Utility class for handling additional functions when loading assets.
 */
UCLASS()
class DYNAMICASSETLOADER_API UAssetUtilities : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Get the full name of an asset.
     * @param Identifier The primary identifier used for lookup.
     * @param Prefix The common prefix shared by all assets of this type.
     * @return The fully combined name
     */
    static FString GetFullAssetName(FStringView Identifier, FStringView Prefix);
};
