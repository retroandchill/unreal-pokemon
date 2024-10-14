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

USTRUCT(BlueprintType)
struct DYNAMICASSETLOADER_API FAssetLoadingEntry {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ContentDir))
    FDirectoryPath RootDirectory;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TOptional<FString> AssetPrefix;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "!bIsNative", HideEditConditionToggle))
    FAssetClassType AssetClass = FAssetClassType(UObject::StaticClass());

    UPROPERTY()
    bool bIsNative = false;

    FAssetLoadingEntry() = default;

  private:
    FAssetLoadingEntry(const FDirectoryPath &RootDirectory, FStringView AssetPrefix,
                       UClass *AssetClass = UObject::StaticClass());
    FAssetLoadingEntry(const FDirectoryPath &RootDirectory, FStringView AssetPrefix, UScriptStruct *AssetClass);

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
     * Edit the defaults for asset classes that are natively defined in code
     */
    UPROPERTY(EditDefaultsOnly, EditFixedSize, BlueprintReadOnly, Category = "Assets", meta = (ReadOnlyKeys))
    TMap<FName, FAssetLoadingEntry> AssetClasses;

    UPROPERTY(EditDefaultsOnly, EditFixedSize, BlueprintReadOnly, Category = "Assets", meta = (ReadOnlyKeys))
    TMap<FName, FAssetLoadingEntry> BlueprintClasses;
};
