// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Templates/NonNullSubclassOf.h"

#include "AssetLoader.generated.h"


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
            StaticLoadObject(T::StaticClass(), nullptr, *SearchKey, nullptr, LOAD_NoWarn));
        } else {
            return UE::Optionals::OfNullable<T>(
            Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *SearchKey, nullptr, LOAD_NoWarn)));   
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
     * @return The found asset
     */
    UFUNCTION(BlueprintCallable, Category = Assets, meta = (CallableWithoutWorldContext,
        DeterminesOutputType = "AssetClass", DynamicOutputParam = "ReturnValue", AutoCreateRefTerm = "BasePackageName"))
    static UObject* LookupAssetByName(UClass* AssetClass, const FDirectoryPath &BasePackageName, const FString &AssetName);

    /**
     * Look up a Blueprint class by its name
     * @tparam T The base class of the blueprint to look for
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @return The found asset
     */
    template <typename T = UObject>
        requires std::is_base_of_v<UObject, T>
    static TOptional<TNonNullSubclassOf<T>> LookupBlueprintClassByName(FStringView BasePackageName, FStringView AssetName) {
        FStringView Prefix;
        if (int32 CharIndex; AssetName.FindLastChar('/', CharIndex)) {
            int32 PrefixLength = CharIndex + 1;
            Prefix = AssetName.SubStr(0, PrefixLength);
            AssetName = AssetName.RightChop(PrefixLength);
        }
        auto SearchKey = FString::Format(TEXT("{0}/{1}{2}.{2}_C"), {BasePackageName, Prefix, AssetName});
        if constexpr (std::is_same_v<T, UObject>) {
            return UE::Optionals::OfNullable(LoadObject<UClass>(nullptr, *SearchKey, nullptr, LOAD_NoWarn)) |
                UE::Optionals::Map([](UClass& Class) { return TNonNullSubclassOf<T>(&Class); });
        } else {
            return UE::Optionals::OfNullable(LoadObject<UClass>(nullptr, *SearchKey, nullptr, LOAD_NoWarn)) |
                UE::Optionals::Filter(&UClass::IsChildOf, T::StaticClass()) |
                UE::Optionals::Map([](UClass& Class) { return TNonNullSubclassOf<T>(&Class); });
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
    static TOptional<TNonNullSubclassOf<T>> LookupBlueprintClassByName(const FDirectoryPath &BasePackageName, FStringView AssetName) {
        return LookupBlueprintClassByName<T>(BasePackageName.Path, AssetName);
    }

    /**
     * Look up a Blueprint class by its name
     * @param BaseClass The base class of the blueprint to look for
     * @param BasePackageName The base package name for the asset
     * @param AssetName The name of the asset itself
     * @return The found asset
     */
    UFUNCTION(BlueprintCallable, Category = Assets, meta = (CallableWithoutWorldContext,
        DeterminesOutputType = "BaseClass", DynamicOutputParam = "ReturnValue", AutoCreateRefTerm = "BasePackageName"))
    static UClass* LookupBlueprintClassByName(UClass* BaseClass, const FDirectoryPath &BasePackageName, const FString &AssetName);
};