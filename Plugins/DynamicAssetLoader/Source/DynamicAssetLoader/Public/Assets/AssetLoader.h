// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Concepts/Types.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/FlatMap.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"
#include "Templates/NonNullSubclassOf.h"

#include "AssetLoader.generated.h"

/**
 * Enum for the result of a dynamic asset load. Uses for flow control in Blueprints.
 */
UENUM()
enum class EAssetLoadResult : uint8 {
    /**
     * The specified asset was found
     */
    Found,

    /**
     * The specified asset was not found
     */
    NotFound,
};

/**
 * Function library for dynamically loading assets.
 */
UCLASS()
class DYNAMICASSETLOADER_API UAssetLoader : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Look up an asset of the given type by its name
     * @tparam T The type of the asset to look up
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @return The found asset
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<T &> LookupAssetByName(FStringView BasePackageName, FStringView AssetName) {
        FStringView Prefix;
        if (int32 CharIndex; AssetName.FindLastChar('/', CharIndex)) {
            int32 PrefixLength = CharIndex + 1;
            Prefix = AssetName.SubStr(0, PrefixLength);
            AssetName = AssetName.RightChop(PrefixLength);
        }

        auto SearchKey = FString::Format(TEXT("{0}/{1}{2}.{2}"), {BasePackageName, Prefix, AssetName});
        if constexpr (std::is_same_v<T, UObject>) {
            return UE::Optionals::OfNullable<T>(
                StaticLoadObject(T::StaticClass(), nullptr, *SearchKey,
                                 nullptr, LOAD_NoWarn));
        } else {
            return UE::Optionals::OfNullable<T>(
                Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *SearchKey,
                                         nullptr, LOAD_NoWarn)));
        }
    }

    /**
     * Look up an asset of the given type by its name
     * @tparam T The type of the asset to look up
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @return The found asset
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<T &> LookupAssetByName(const FDirectoryPath &BasePackageName, FStringView AssetName) {
        return LookupAssetByName<T>(BasePackageName.Path, AssetName);
    }

    /**
     * Look up an asset of the given type by its name
     * @param AssetClass The type of the asset to look up
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @param FoundAsset The found asset
     * @return The result of the lookup
     */
    UFUNCTION(BlueprintCallable, Category = Assets, meta = (CallableWithoutWorldContext,
        DeterminesOutputType = "AssetClass", DynamicOutputParam = "FoundAsset", AutoCreateRefTerm = "BasePackageName",
        ExpandEnumAsExecs = "ReturnValue"))
    static EAssetLoadResult LookupAssetByName(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                              const FString &AssetName, UObject *&FoundAsset);

    UFUNCTION(BlueprintInternalUseOnly, meta = (CallableWithoutWorldContext,
        DeterminesOutputType = "AssetClass", DynamicOutputParam = "FoundAsset"))
    static EAssetLoadResult LoadDynamicAsset(FName Identifier, const FString& AssetName, UObject*& FoundAsset);

    /**
     * Look up a Blueprint class by its name
     * @tparam T The base class of the blueprint to look for
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @return The found asset
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<TNonNullSubclassOf<T>> LookupBlueprintClassByName(FStringView BasePackageName,
                                                                       FStringView AssetName) {
        FStringView Prefix;
        if (int32 CharIndex; AssetName.FindLastChar('/', CharIndex)) {
            int32 PrefixLength = CharIndex + 1;
            Prefix = AssetName.SubStr(0, PrefixLength);
            AssetName = AssetName.RightChop(PrefixLength);
        }
        auto SearchKey = FString::Format(TEXT("{0}/{1}{2}.{2}_C"), {BasePackageName, Prefix, AssetName});
        if constexpr (std::is_same_v<T, UObject>) {
            // clang-format off
            return UE::Optionals::OfNullable(LoadObject<UClass>(nullptr, *SearchKey, nullptr, LOAD_NoWarn)) |
                   UE::Optionals::Map([](UClass &Class) {
                       return TNonNullSubclassOf<T>(&Class);
                   });
            // clang-format on
        } else {
            // clang-format off
            return UE::Optionals::OfNullable(LoadObject<UClass>(nullptr, *SearchKey, nullptr, LOAD_NoWarn)) |
                   UE::Optionals::Filter(&UClass::IsChildOf, T::StaticClass()) |
                   UE::Optionals::Map([](UClass &Class) {
                       return TNonNullSubclassOf<T>(&Class);
                   });
            // clang-format on
        }
    }

    /**
     * Look up a Blueprint class by its name
     * @tparam T The base class of the blueprint to look for
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @return The found asset
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<TNonNullSubclassOf<T>> LookupBlueprintClassByName(const FDirectoryPath &BasePackageName,
                                                                       FStringView AssetName) {
        return LookupBlueprintClassByName<T>(BasePackageName.Path, AssetName);
    }

    /**
     * Look up a Blueprint class by its name
     * @param BaseClass The base class of the blueprint to look for
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @param FoundClass The found class
     * @return The result of the lookup
     */
    UFUNCTION(BlueprintCallable, Category = Assets, meta = (CallableWithoutWorldContext,
        DeterminesOutputType = "BaseClass", DynamicOutputParam = "FoundClass", AutoCreateRefTerm = "BasePackageName",
        ExpandEnumAsExecs="ReturnValue"))
    static EAssetLoadResult LookupBlueprintClassByName(UClass *BaseClass, const FDirectoryPath &BasePackageName,
                                                       const FString &AssetName, UClass *&FoundClass);

    /**
     * Fetch the first matching asset for the provided keys
     * @tparam T The type of asset we're looking for
     * @param BasePackageName The base package to search for when resolving
     * @param Keys The keys to use when resolving
     * @return The found asset, if it exists
     */
    template <typename T = UObject, typename R>
        requires std::is_base_of_v<UObject, T> && UE::Ranges::Range<R>
                 && std::convertible_to<UE::Ranges::TRangeCommonReference<R>, FStringView>
    static TOptional<T &> ResolveAsset(FStringView BasePackageName, R &&Keys) {
        using ElementType = UE::Ranges::TRangeCommonReference<R>;
        // clang-format off
        return Keys |
               UE::Ranges::Map([&BasePackageName](ElementType Key) {
                   return LookupAssetByName<T>(BasePackageName, Key);
               }) |
               UE::Ranges::Filter([](TOptional<T&> Optional) { return Optional.IsSet(); }) |
               UE::Ranges::FindFirst |
               UE::Optionals::FlatMap([](const TOptional<T &> Optional) {
                   return Optional;
               });
        // clang-format on
    }

    /**
     * Fetch the first matching asset for the provided keys
     * @tparam T The type of asset we're looking for
     * @param BasePackageName The base package to search for when resolving
     * @param Keys The keys to use when resolving
     * @return The found asset, if it exists
     */
    template <typename T = UObject, typename R>
        requires std::is_base_of_v<UObject, T> && UE::Ranges::Range<R>
                 && std::convertible_to<UE::Ranges::TRangeCommonReference<R>, FStringView>
    static TOptional<T &> ResolveAsset(const FDirectoryPath &BasePackageName, R &&Keys) {
        return ResolveAsset<T, R>(BasePackageName.Path, Forward<R>(Keys));
    }

    /**
     * Fetch the first matching asset for the provided keys
     * @param AssetClass The type of asset we're looking for
     * @param BasePackageName The base package to search for when resolving
     * @param Keys The keys to use when resolving
     * @param FoundAsset The found asset, if it exists
     * @return The result of the lookup
     */
    UFUNCTION(BlueprintCallable, Category = Assets, meta = (CallableWithoutWorldContext,
        DeterminesOutputType = "BaseClass", DynamicOutputParam = "FoundAsset", AutoCreateRefTerm = "BasePackageName",
        ExpandEnumAsExecs="ReturnValue"))
    static EAssetLoadResult ResolveAsset(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                         const TArray<FString> &Keys, UObject *&FoundAsset);
};