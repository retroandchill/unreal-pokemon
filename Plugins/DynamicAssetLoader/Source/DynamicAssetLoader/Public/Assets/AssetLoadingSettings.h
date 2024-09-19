// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AssetLoadingSettings.generated.h"

namespace UE::Assets {
    template <typename T>
        requires std::is_base_of_v<UObject, T>
    class TAssetClass;
}

USTRUCT(BlueprintType)
struct DYNAMICASSETLOADER_API FAssetLoadingEntry {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ContentDir))
	FDirectoryPath RootDirectory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TOptional<FString> AssetPrefix;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "!bIsNative"))
    UClass* AssetClass = nullptr;

    UPROPERTY()
    bool bIsNative = false;

    FAssetLoadingEntry() = default;

private:
    FAssetLoadingEntry(const FDirectoryPath& RootDirectory, FStringView AssetPrefix, UClass* AssetClass);

    template <typename T>
        requires std::is_base_of_v<UObject, T>
    friend class UE::Assets::TAssetClass;
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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (ReadOnlyKeys, EditFixedSize))
    TMap<FName, FAssetLoadingEntry> NativeAssetClasses;

    /**
     * Asset classes that are defined outside the codebase
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (ReadOnlyKeys, EditFixedSize))
    TMap<FName, FAssetLoadingEntry> UserCreatedAssetClasses;

};
