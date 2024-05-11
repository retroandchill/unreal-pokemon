// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "TypedAssetRepository.h"
#include "Engine/DataAsset.h"
#include "StaticImageRepository.generated.h"

/**
 * Represents a repository that can contain any valid image asset type.
 */
UCLASS()
class POKEMONASSETS_API UStaticImageRepository final : public UDataAsset,
#if CPP
                                                 public TTypedAssetRepository<UObject> {
#else
                                                 public IAssetRepository {
#endif
    GENERATED_BODY()

protected:
    bool AssetValid(const FAssetData &AssetData) const override;

    TMap<FName, TSoftObjectPtr<>> &GetAssetMap() override;
    const TMap<FName, TSoftObjectPtr<>> &GetAssetMap() const override;
    FStringView GetNamePrefix() const override;

private:
                                                     UPROPERTY(EditAnywhere, Category = Lookup)
                                                     FString NamePrefix;
                                                     
    /**
     * The map of name keys to their corresponding Assets
     */
    UPROPERTY(VisibleAnywhere, Category = Lookup)
    TMap<FName, TSoftObjectPtr<UObject>> AssetMap;

};