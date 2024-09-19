// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AssetLoader.h"
#include "AssetLoadingSettings.h"
#include "AssetUtilities.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/Map.h"

namespace UE::Assets {

    template <typename T>
    concept AssetKey = std::is_same_v<std::remove_cvref_t<T>, FName> || std::is_convertible_v<T, FStringView>;

    /**
     * Class used for native declarations of asset classes
     * @tparam T The class of the asset in question
     */
    template <typename T>
        requires std::is_base_of_v<UObject, T>
    class TAssetClass {
    public:
        /**
         * Construct a new asset object using this loader
         * @param Key The key for the asset in question
         * @param DefaultPath The default path to the asset, should be a valid game directory
         * @param DefaultPrefix The default prefix for the asset type
         */
        explicit TAssetClass(FName Key, FStringView DefaultPath, FStringView DefaultPrefix = TEXT(""))
            : Key(Key), DefaultAssetPath(FString(DefaultPath)), DefaultPrefix(DefaultPrefix) {
            FCoreDelegates::OnPostEngineInit.AddRaw(this, &TAssetClass::OnPostEngineInit);
        }

        /**
         * Load the asset with the specified name
         * @param AssetName The name of the asset in question
         * @return The loaded asset
         */
        TOptional<T &> LoadAsset(FStringView AssetName) const {
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->NativeAssetClasses.FindChecked(Key);
            auto FullName = UAssetUtilities::GetFullAssetName(AssetName,
                                                              AssetClassData.AssetPrefix.Get(TEXT("")));
            return UAssetLoader::LookupAssetByName<T>(AssetClassData.RootDirectory, FullName);
        }

        /**
         * Load the asset with the specified name
         * @param AssetName The name of the asset in question
         * @return The loaded asset
         */
        TOptional<T &> LoadAsset(FName AssetName) const {
            return LoadAsset(AssetName.ToString());
        }

        template <typename R>
            requires Ranges::Range<R> && std::convertible_to<Ranges::TRangeCommonReference<R>, FStringView>
        TOptional<T &> ResolveAsset(R &&Assets) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->NativeAssetClasses.FindChecked(Key);
            auto Prefix = AssetClassData.AssetPrefix.Get(TEXT(""));
            // clang-format off
            auto FullNames = Assets |
                             Ranges::Map([&Prefix](ElementType AssetName) {
                                 return UAssetUtilities::GetFullAssetName(AssetName, Prefix);
                             });
            // clang-format on
            return UAssetLoader::ResolveAsset<T>(AssetClassData.RootDirectory, FullNames);
        }

        template <typename R>
            requires Ranges::Range<R> && AssetKey<Ranges::TRangeCommonReference<R>>
        TArray<TOptional<T &>> LoadAssets(R &&Assets) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            // clang-format off
            return Assets |
                   Ranges::Map([this](ElementType Value) { return LoadAsset(Value); }) |
                   Ranges::ToArray;
            // clang-format on
        }

    private:
        void OnPostEngineInit() {
            auto Settings = GetMutableDefault<UAssetLoadingSettings>();
            if (Settings->NativeAssetClasses.Contains(Key)) {
                return;
            }

            Settings->NativeAssetClasses.Emplace(
                Key, FAssetLoadingEntry(DefaultAssetPath, DefaultPrefix, T::StaticClass()));
        }

        FName Key;
        FDirectoryPath DefaultAssetPath;
        FStringView DefaultPrefix;
    };
}

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_ASSET_CLASS(Name, AssetType) \
    const UE::Assets::TAssetClass<AssetType> Name

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_ASSET_CLASS_EXTERN(Name, AssetType) \
    extern const UE::Assets::TAssetClass<AssetType> Name

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_ASSET_CLASS_EXPORTED(ExportSymbol, Name, AssetType) \
    ExportSymbol extern const UE::Assets::TAssetClass<AssetType> Name

/**
 * Define an asset class to use for loading assets.
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 * @param Directory The default directory for the asset class
 * @param Prefix The default prefix for the asset
 */
#define UE_DEFINE_ASSET_CLASS(Name, AssetType, Directory, Prefix) \
    const UE::Assets::TAssetClass<AssetType> Name(#Name, TEXT(Directory), TEXT(Prefix))

/**
 * Define an asset class to use for loading assets.
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 * @param Directory The default directory for the asset class
 * @param Prefix The default prefix for the asset
 */
#define UE_DEFINE_ASSET_CLASS_STATIC(Name, AssetType, Directory, Prefix) \
    static const UE::Assets::TAssetClass<AssetType> Name(#Name, TEXT(Directory), TEXT(Prefix))