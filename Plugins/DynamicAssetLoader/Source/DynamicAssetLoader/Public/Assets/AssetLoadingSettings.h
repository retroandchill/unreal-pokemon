// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Variants/SoftVariantObject.h"
#include "Ranges/Variants/VariantObject.h"
#include "Ranges/Variants/VariantObjectStruct.h"

#include "AssetLoadingSettings.generated.h"

namespace UE::Assets {
    template <typename T>
    concept AssetClassType = std::is_base_of_v<UObject, T> || Ranges::VariantObjectStruct<T>;

    template <typename T>
        requires AssetClassType<T>
    class TAssetClass;

    template <typename T>
        requires std::is_base_of_v<UObject, T>
    class TBlueprintClass;
} // namespace UE::Assets

#if CPP
UE_DECLARE_VARIANT_OBJECT_STRUCT(AssetClassType, UClass, UScriptStruct);
#else
USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FAssetClassType {
    UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/Engine.UClass,/Script/Engine.ScriptStruct"))
    TObjectPtr<UObject> ContainedObject;

    UPROPERTY()
    uint64 TypeIndex;
};

USTRUCT(BlueprintType, NoExport, meta = (HiddenByDefault, DisableSplitPin))
struct FSoftAssetClassType {
    UPROPERTY(EditAnywhere, meta = (AllowedClasses = "/Script/Engine.UClass,/Script/Engine.ScriptStruct"))
    TSoftObjectPtr<UObject> Ptr;

    UPROPERTY()
    uint64 TypeIndex;
};
#endif

template <>
struct DYNAMICASSETLOADER_API TBaseStructure<FAssetClassType> {
    static UScriptStruct *Get();
};

template <>
struct DYNAMICASSETLOADER_API TBaseStructure<FSoftAssetClassType> {
    static UScriptStruct *Get();
};

/**
 * Represents an entry in the asset loading system. This class encapsulates
 * details about an asset that is to be loaded, potentially including its
 * resource identifier, load priority, and any other metadata that assists
 * in the asset management process.
 */
USTRUCT(BlueprintType)
struct DYNAMICASSETLOADER_API FAssetLoadingEntry {
    GENERATED_BODY()

    /**
     * DisplayName represents the user-friendly name corresponding to an asset or
     * entry in the system. This variable is typically used for display purposes
     * in the UI or for localization.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    /**
     * Represents the root directory path where the assets are stored. This variable
     * is intended to be used by the asset loading system to locate and manage assets.
     * The path specified here serves as the base directory for asset search and loading operations.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ContentDir))
    FDirectoryPath RootDirectory;

    /**
     * Specifies an optional prefix for asset names within the asset loading system.
     * This prefix, if provided, will be prepended to asset names to form a complete
     * path or identifier during asset management and location processes.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TOptional<FString> AssetPrefix;

    /**
     * Represents the class type of the asset in the asset loading system. This variable
     * holds the type information for the asset, which may be necessary for various operations
     * such as loading, casting, and asset management. The asset class type can be specified
     * conditionally and can be edited within the asset editor, unless the asset is marked as native.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "!bIsNative", HideEditConditionToggle))
    FAssetClassType AssetClass = FAssetClassType(UObject::StaticClass());

    /**
     * Indicates whether the asset is a native object. Native objects are typically
     * built into the application and are not subject to certain dynamic operations
     * such as asset class editing in the user interface. This flag can be used to
     * apply conditional logic during asset management and manipulation processes.
     */
    UPROPERTY()
    bool bIsNative = false;

    FAssetLoadingEntry() = default;

  private:
    FAssetLoadingEntry(FName Key, FStringView RootDirectory, FStringView AssetPrefix,
                       UClass *AssetClass = UObject::StaticClass());
    FAssetLoadingEntry(FName Key,FStringView RootDirectory, FStringView AssetPrefix, UScriptStruct *AssetClass);

    template <typename T>
        requires UE::Assets::AssetClassType<T>
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
     * A mapping between asset identifiers and their corresponding asset loading
     * entries. This TMap is used within the asset loading system to store configurations
     * for different types of assets, encapsulating details such as the display name,
     * directory path, and optional prefix for the assets. The map's keys are of type FName,
     * while the values are FAssetLoadingEntry objects. This setting is read-only within
     * Blueprints and has a fixed size, meaning entries cannot be added or removed at runtime.
     * It is configured in the project settings under the "Assets" category.
     */
    UPROPERTY(EditDefaultsOnly, EditFixedSize, BlueprintReadOnly, Config, Category = "Assets", meta = (ReadOnlyKeys))
    TMap<FName, FAssetLoadingEntry> AssetClasses;

    /**
     * A mapping between asset identifiers and their corresponding asset loading
     * entries. This TMap is specifically used for Blueprint classes and provides
     * read-only access to the asset details. Each entry in the map includes
     * information necessary for asset management, such as display name, directory
     * path, and optional asset prefix. These entries are configured in the asset
     * loading settings and are fixed in size, meaning new entries cannot be added or
     * removed at runtime. It is configured in the project settings under the "Assets" category.
     */
    UPROPERTY(EditDefaultsOnly, EditFixedSize, BlueprintReadOnly, Config, Category = "Assets", meta = (ReadOnlyKeys))
    TMap<FName, FAssetLoadingEntry> BlueprintClasses;
};
