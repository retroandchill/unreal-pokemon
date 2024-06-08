// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TypedAssetRepository.h"

#include "TextureRepository.generated.h"

/**
 * Repository for holding a texture 2D asset.
 */
UCLASS()
class POKEMONASSETS_API UTextureRepository final : public UDataAsset,
#if CPP
                                                   public TTypedAssetRepository<UTexture2D> {
#else
                                                   public IAssetRepository {
#endif
    GENERATED_BODY()

  protected:
    TMap<FName, TSoftObjectPtr<UTexture2D>> &GetAssetMap() override;
    const TMap<FName, TSoftObjectPtr<UTexture2D>> &GetAssetMap() const override;
    FStringView GetNamePrefix() const override;

  private:
    UPROPERTY(EditAnywhere, Category = Lookup)
    FString NamePrefix;

    /**
     * The map of name keys to their corresponding textures
     */
    UPROPERTY(VisibleAnywhere, Category = Lookup)
    TMap<FName, TSoftObjectPtr<UTexture2D>> TextureMap;
};
