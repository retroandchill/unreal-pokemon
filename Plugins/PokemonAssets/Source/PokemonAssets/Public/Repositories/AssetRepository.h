// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "AssetRepository.generated.h"

// This class does not need to be modified.
UINTERFACE()
class POKEMONASSETS_API UAssetRepository : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface for a declaration of a registered asset type used by the game
 */
class POKEMONASSETS_API IAssetRepository {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
#if WITH_EDITOR
    /**
     * Register the provided asset within the game
     * @param AssetData The asset in question to register
     */
    virtual void RegisterAsset(const FAssetData &AssetData) = 0;

    /**
     * Unregister the provided asset within the game
     * @param AssetData The asset in question to remove
     */
    virtual void UnregisterAsset(const FAssetData &AssetData) = 0;
#endif
};