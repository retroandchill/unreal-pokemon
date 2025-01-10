// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/Filter.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/FindFirst.h"

#include "RetroLib/Ranges/Views/NameAliases.h"
#include "RetroLib/Utils/SoftObjectRef.h"
#include "Templates/NonNullSubclassOf.h"
#include "TextureCompiler.h"

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
     * Attempt to load an asset, completing all texture compilation in an editor build.
     * @tparam T The type of asset to load
     * @param SearchKey The search key for the asset
     * @return The loaded asset
     */
    template <typename T>
        requires std::is_base_of_v<UObject, T>
    static TOptional<T &> AttemptLoad(FStringView SearchKey) {
        if constexpr (std::is_same_v<T, UObject>) {
            auto Asset = Retro::Optionals::OfNullable(
                StaticLoadObject(T::StaticClass(), nullptr, SearchKey.GetData(), nullptr, LOAD_NoWarn));

#if WITH_EDITOR
            FTextureCompilingManager::Get().FinishAllCompilation();
#endif
            return Asset;
        } else {
            auto Asset = Retro::Optionals::OfNullable(
                Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, SearchKey.GetData(), nullptr, LOAD_NoWarn)));

#if WITH_EDITOR
            FTextureCompilingManager::Get().FinishAllCompilation();
#endif
            return Asset;
        }
    }

    /**
     * Creates a unique search key based on the provided base package name and asset name.
     * The search key is constructed by formatting and combining these elements.
     *
     * @param BasePackageName The base package name to be used in the search key.
     * @param AssetName The asset name to be used in the search key, which may contain a prefix.
     * @return A combined string that represents the unique search key.
     */
    static FString CreateSearchKey(FStringView BasePackageName, FStringView AssetName);

    /**
     * Look up an asset of the given type by its name
     * @tparam T The type of the asset to look up
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @return The found asset
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<T &> FindAssetByName(FStringView BasePackageName, FStringView AssetName) {
        return AttemptLoad<T>(CreateSearchKey(BasePackageName, AssetName));
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
    static TOptional<T &> FindAssetByName(const FDirectoryPath &BasePackageName, FStringView AssetName) {
        return FindAssetByName<T>(BasePackageName.Path, AssetName);
    }

    /**
     * Looks up an asset by its base package name and asset name, returning an optional soft object reference if found.
     *
     * @param BasePackageName The base package name to search within.
     * @param AssetName The name of the asset to search for.
     * @return An optional soft object reference to the asset if found; otherwise, an empty optional.
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<TSoftObjectRef<T>> LookupAssetByName(FStringView BasePackageName, FStringView AssetName) {
        const auto &AssetManager = UAssetManager::Get();
        FSoftObjectPath Path(CreateSearchKey(BasePackageName, AssetName));
        if (FAssetData AssetData; AssetManager.GetAssetDataForPath(Path, AssetData) && AssetData.IsInstanceOf<T>()) {
            return Retro::Optionals::OfNullable(TSoftObjectPtr<T>(Path));
        }

        return TOptional<TSoftObjectRef<T>>();
    }

    /**
     * Looks up an asset by its name within a specified base package.
     *
     * @param BasePackageName The directory path of the base package where the asset is located.
     * @param AssetName The name of the asset to be looked up.
     * @return An optional reference to the found asset, wrapped in TOptional.
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<TSoftObjectRef<T>> LookupAssetByName(const FDirectoryPath &BasePackageName,
                                                          FStringView AssetName) {
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
    UFUNCTION(BlueprintCallable, Category = Assets,
              meta = (CallableWithoutWorldContext, DeterminesOutputType = "AssetClass",
                      DynamicOutputParam = "FoundAsset", AutoCreateRefTerm = "BasePackageName",
                      ExpandEnumAsExecs = "ReturnValue"))
    static EAssetLoadResult FindAssetByName(const UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                            const FString &AssetName, UObject *&FoundAsset);

    /**
     * This method looks up an asset by name given a base package name and asset class.
     *
     * @param AssetClass The class type of the asset being searched for.
     * @param BasePackageName The base directory path where the asset is located.
     * @param AssetName The name of the asset to look up.
     * @param FoundAsset A reference to a TSoftObjectPtr<> that will hold the located asset if found.
     *
     * @return Returns an EAssetLoadResult indicating whether the asset was found or not.
     */
    UFUNCTION(BlueprintCallable, Category = Assets,
              meta = (CallableWithoutWorldContext, DeterminesOutputType = "AssetClass",
                      DynamicOutputParam = "FoundAsset", AutoCreateRefTerm = "BasePackageName",
                      ExpandEnumAsExecs = "ReturnValue"))
    static EAssetLoadResult LookupAssetByName(const UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                              const FString &AssetName, TSoftObjectPtr<UObject> &FoundAsset);

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, CustomThunk,
              meta = (CallableWithoutWorldContext, ExpandEnumAsExecs = "ReturnValue",
                      CustomStructureParam = "AssetName,FoundAsset"))
    static EAssetLoadResult LoadDynamicAsset(FName Identifier, const FString &AssetName, UObject *&FoundAsset);
    DECLARE_FUNCTION(execLoadDynamicAsset);

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, CustomThunk,
              meta = (CallableWithoutWorldContext, ExpandEnumAsExecs = "ReturnValue",
                      CustomStructureParam = "AssetName,FoundAsset"))
    static EAssetLoadResult LookupDynamicAsset(FName Identifier, const FString &AssetName,
                                               TSoftObjectPtr<UObject> &FoundAsset);
    DECLARE_FUNCTION(execLookupDynamicAsset);

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
            return Retro::Optionals::OfNullable(LoadObject<UClass>(nullptr, *SearchKey, nullptr, LOAD_NoWarn)) |
                   Retro::Optionals::Transform([](UClass &Class) {
                       return TNonNullSubclassOf<T>(&Class);
                   });
            // clang-format on
        } else {
            // clang-format off
            return Retro::Optionals::OfNullable(LoadObject<UClass>(nullptr, *SearchKey, nullptr, LOAD_NoWarn)) |
                   Retro::Optionals::Filter([](UClass& Class) { return Class.IsChildOf(T::StaticClass()); }) |
                   Retro::Optionals::Transform([](UClass &Class) {
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
    UFUNCTION(BlueprintCallable, Category = Assets,
              meta = (CallableWithoutWorldContext, DeterminesOutputType = "BaseClass",
                      DynamicOutputParam = "FoundClass", AutoCreateRefTerm = "BasePackageName",
                      ExpandEnumAsExecs = "ReturnValue"))
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
        requires std::is_base_of_v<UObject, T> && std::ranges::input_range<R> &&
                 std::convertible_to<Retro::TRangeCommonReference<R>, FStringView>
    static TOptional<T &> ResolveAsset(FStringView BasePackageName, R &&Keys) {
        // clang-format off
        return Keys |
               Retro::Ranges::Views::Transform([&BasePackageName]<typename U>(U &&Key) {
                   return FindAssetByName<T>(BasePackageName, std::forward<U>(Key));
               }) |
               Retro::Ranges::Views::Join |
               Retro::Ranges::FindFirst() |
               Retro::Optionals::AndThen([](const TOptional<T &> Optional) {
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
        requires std::is_base_of_v<UObject, T> && std::ranges::input_range<R> &&
                 std::convertible_to<Retro::TRangeCommonReference<R>, FStringView>
    static TOptional<T &> ResolveAsset(const FDirectoryPath &BasePackageName, R &&Keys) {
        return ResolveAsset<T, R>(BasePackageName.Path, std::forward<R>(Keys));
    }

    /**
     * Fetch the first matching asset for the provided keys
     * @param AssetClass The type of asset we're looking for
     * @param BasePackageName The base package to search for when resolving
     * @param Keys The keys to use when resolving
     * @param FoundAsset The found asset, if it exists
     * @return The result of the lookup
     */
    UFUNCTION(BlueprintCallable, Category = Assets,
              meta = (CallableWithoutWorldContext, DeterminesOutputType = "BaseClass",
                      DynamicOutputParam = "FoundAsset", AutoCreateRefTerm = "BasePackageName",
                      ExpandEnumAsExecs = "ReturnValue"))
    static EAssetLoadResult ResolveAsset(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                         const TArray<FString> &Keys, UObject *&FoundAsset);

    /**
     * Resolves a soft asset reference by searching through a range of keys.
     *
     * This function uses a pipeline of operations including mapping, filtering,
     * finding the first occurrence, and flattening optional values to locate
     * and return the first resolved soft asset reference.
     *
     * @tparam T Type of the asset to resolve.
     * @tparam R Type of the range of keys.
     * @param BasePackageName The base package name used for asset lookup.
     * @param Keys A range of keys to search through.
     * @return An optional containing the first found soft asset reference or an unset optional if none are found.
     */
    template <typename T = UObject, typename R>
        requires std::is_base_of_v<UObject, T> && std::ranges::input_range<R> &&
                 std::convertible_to<Retro::TRangeCommonReference<R>, FStringView>
    static TOptional<TSoftObjectRef<T>> ResolveSoftAsset(FStringView BasePackageName, R &&Keys) {
        using ElementType = Retro::TRangeCommonReference<R>;
        // clang-format off
        return Keys |
               Retro::Ranges::Views::Transform([&BasePackageName](ElementType Key) {
                   return LookupAssetByName<T>(BasePackageName, Key);
               }) |
               Retro::Ranges::Views::Filter([](TOptional<TSoftObjectRef<T>> Optional) { return Optional.IsSet(); }) |
               Retro::Ranges::FindFirst() |
               Retro::Optionals::AndThen([](const TOptional<TSoftObjectRef<T>> Optional) {
                   return Optional;
               });
        // clang-format on
    }

    /**
     * Resolves a soft asset reference given a base package name and a set of keys.
     *
     * @param BasePackageName The base package name to resolve the asset from.
     * @param Keys A set of keys used for resolving the asset.
     * @return An optional soft object reference to the resolved asset.
     */
    template <typename T = UObject, typename R>
        requires std::is_base_of_v<UObject, T> && std::ranges::input_range<R> &&
                 std::convertible_to<Retro::TRangeCommonReference<R>, FStringView>
    static TOptional<TSoftObjectRef<T>> ResolveSoftAsset(const FDirectoryPath &BasePackageName, R &&Keys) {
        return ResolveSoftAsset<T, R>(BasePackageName.Path, std::forward<R>(Keys));
    }

    /**
     * Fetch the first matching asset for the provided keys
     * @tparam T The type of asset we're looking for
     * @param BasePackageName The base package to search for when resolving
     * @param Keys The keys to use when resolving
     * @return The found asset, if it exists
     */
    template <typename T = UObject, typename R>
        requires std::is_base_of_v<UObject, T> && std::ranges::input_range<R> &&
                 std::convertible_to<Retro::TRangeCommonReference<R>, FStringView>
    static TOptional<TNonNullSubclassOf<T>> ResolveClass(FStringView BasePackageName, R &&Keys) {
        using ElementType = Retro::TRangeCommonReference<R>;
        // clang-format off
        return Keys |
               Retro::Ranges::Views::Transform([&BasePackageName](ElementType Key) {
                   return LookupBlueprintClassByName<T>(BasePackageName, Key);
               }) |
               Retro::Ranges::Views::Filter([](TOptional<TNonNullSubclassOf<T>> Optional) { return Optional.IsSet(); }) |
               Retro::Ranges::FindFirst() |
               Retro::Optionals::AndThen([](const TOptional<TNonNullSubclassOf<T>> Optional) {
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
        requires std::is_base_of_v<UObject, T> && std::ranges::input_range<R> &&
                 std::convertible_to<Retro::TRangeCommonReference<R>, FStringView>
    static TOptional<TNonNullSubclassOf<T>> ResolveClass(const FDirectoryPath &BasePackageName, R &&Keys) {
        return ResolveClass<T, R>(BasePackageName.Path, std::forward<R>(Keys));
    }

    /**
     * Fetch the first matching asset for the provided keys
     * @param AssetClass The type of asset we're looking for
     * @param BasePackageName The base package to search for when resolving
     * @param Keys The keys to use when resolving
     * @param FoundClass The found asset, if it exists
     * @return The result of the lookup
     */
    UFUNCTION(BlueprintCallable, Category = Assets,
              meta = (CallableWithoutWorldContext, DeterminesOutputType = "BaseClass",
                      DynamicOutputParam = "FoundAsset", AutoCreateRefTerm = "BasePackageName",
                      ExpandEnumAsExecs = "ReturnValue"))
    static EAssetLoadResult ResolveClass(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                         const TArray<FString> &Keys, UClass *&FoundClass);
};