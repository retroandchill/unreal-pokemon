// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Ranges/RangeConcepts.h"

#include "AssetLoadingSettings.generated.h"

namespace UE::Assets {
    template <typename T>
        requires std::is_base_of_v<UObject, T>
    class TAssetClass;

    template <typename T>
        requires std::is_base_of_v<UObject, T>
    class TBlueprintClass;
} // namespace UE::Assets

USTRUCT(BlueprintType)
struct DYNAMICASSETLOADER_API FAssetLoadingEntry {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ContentDir))
    FDirectoryPath RootDirectory;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TOptional<FString> AssetPrefix;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "!bIsNative", HideEditConditionToggle))
    UClass *AssetClass = nullptr;

    UPROPERTY()
    bool bIsNative = false;

    FAssetLoadingEntry() = default;

  private:
    FAssetLoadingEntry(const FDirectoryPath &RootDirectory, FStringView AssetPrefix, UClass *AssetClass);

    template <typename T>
        requires std::is_base_of_v<UObject, T>
    friend class UE::Assets::TAssetClass;

    template <typename T>
        requires std::is_base_of_v<UObject, T>
    friend class UE::Assets::TBlueprintClass;
};

/**
 * Settings related to the dynamic loading of assets by the game.
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Asset Loading")
class DYNAMICASSETLOADER_API UAssetLoadingSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Edit the defaults for asset classes that are natively defined in code
     */
    UPROPERTY(EditDefaultsOnly, EditFixedSize, BlueprintReadOnly, Category = "Assets", meta = (ReadOnlyKeys))
    TMap<FName, FAssetLoadingEntry> AssetClasses;

    UPROPERTY(EditDefaultsOnly, EditFixedSize, BlueprintReadOnly, Category = "Assets", meta = (ReadOnlyKeys))
    TMap<FName, FAssetLoadingEntry> BlueprintClasses;
};
