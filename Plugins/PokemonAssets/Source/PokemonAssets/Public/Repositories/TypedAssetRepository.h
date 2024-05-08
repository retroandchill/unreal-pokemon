// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AssetRepository.h"

/**
 * Templated class used by the internals to handle common functionality.
 * @tparam T The type of asset being loaded
 */
template <typename T>
    requires std::is_base_of_v<UObject, T>
class TTypedAssetRepository : public IAssetRepository {
  public:
    /**
     * Fetch the given asset with the provided key
     * @param Key The key to use for the asset in question
     * @return The asset (if found)
     */
    T *FetchAsset(FName Key) const {
        auto Lookup = GetAssetMap().Find(Key);
        return Lookup != nullptr ? Lookup->Get() : nullptr;
    }

    /**
     * Fetch the first matching asset for the provided keys
     * @param Keys The keys to use for the assets in question
     * @return The asset (if found)
     */
    T *ResolveAsset(const TArray<FName> &Keys) const {
        auto &AssetMap = GetAssetMap();
        for (const auto &Key : Keys) {
            auto Lookup = AssetMap.Find(Key);
            if (Lookup == nullptr) {
                continue;
            }

            return Lookup->Get();
        }

        return nullptr;
    }

#if WITH_EDITOR
    void SetBasePackage(FStringView PackageName) final {
        BasePackageName = PackageName;
    }
    
    void RegisterAsset(const FAssetData &AssetData) override {
        if (AssetData.GetClass(EResolveClass::Yes) == T::StaticClass()) {
            auto FolderName = AssetData.PackagePath.ToString();
            FolderName.RemoveFromStart(BasePackageName);
            FolderName.RemoveFromStart(TEXT("/"));
            auto AssetName = FolderName.IsEmpty() ? AssetData.AssetName : FName(*FString::Format(TEXT("{0}/{1}"),
                {FolderName, AssetData.AssetName.ToString()}));
            GetAssetMap().Add(AssetName, AssetData.GetAsset());
        }
    }

    void UnregisterAsset(const FAssetData &AssetData) override {
        if (GetAssetMap().Contains(AssetData.AssetName)) {
            GetAssetMap().Remove(AssetData.AssetName);
        }
    }
#endif

  protected:
    /**
     * Get the map of names to the actual assets.
     * @return The map of names to the actual assets.
     */
    virtual TMap<FName, TSoftObjectPtr<T>> &GetAssetMap() = 0;

    /**
     * Get the map of names to the actual assets.
     * @return The map of names to the actual assets.
     */
    virtual const TMap<FName, TSoftObjectPtr<T>> &GetAssetMap() const = 0;

private:
#if WITH_EDITOR
    FString BasePackageName;
#endif
    
};
