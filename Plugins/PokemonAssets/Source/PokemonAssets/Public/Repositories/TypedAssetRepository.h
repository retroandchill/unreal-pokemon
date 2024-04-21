// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AssetRepository.h"

/**
 * Templated class used by the internals to handle common functionality.
 */
template <typename T>
requires std::is_base_of_v<UObject, T>
class TTypedAssetRepository : public IAssetRepository {
public:
    T* FetchAsset(FName Key) const {
        auto Lookup = GetAssetMap().Find(Key);
        return Lookup != nullptr ? Lookup->Get() : nullptr;
    }
    
#if WITH_EDITOR
    void RegisterAsset(const FAssetData &AssetData) override {
        if (AssetData.GetClass(EResolveClass::Yes) == T::StaticClass()) {
            GetAssetMap().Add(AssetData.AssetName, AssetData.GetAsset());
        }
    }
    
    void UnregisterAsset(const FAssetData &AssetData) override {
        if (GetAssetMap().Contains(AssetData.AssetName)) {
            GetAssetMap().Remove(AssetData.AssetName);
        }
    }
#endif

protected:
    virtual TMap<FName, TSoftObjectPtr<T>> &GetAssetMap() = 0;
    virtual const TMap<FName, TSoftObjectPtr<T>> &GetAssetMap() const = 0;

};
