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
        template <typename U = T>
            requires std::is_base_of_v<T, U>
        TOptional<U &> LoadAsset(FStringView AssetName) const {
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->AssetClasses.FindChecked(Key);
            auto FullName = UAssetUtilities::GetFullAssetName(AssetName, AssetClassData.AssetPrefix.Get(TEXT("")));
            return UAssetLoader::LookupAssetByName<U>(AssetClassData.RootDirectory, FullName);
        }

        /**
         * Load the asset with the specified name
         * @param AssetName The name of the asset in question
         * @return The loaded asset
         */
        template <typename U = T>
            requires std::is_base_of_v<T, U>
        TOptional<U &> LoadAsset(FName AssetName) const {
            return LoadAsset<U>(AssetName.ToString());
        }

        template <typename U = T, typename R>
            requires std::is_base_of_v<T, U> && Ranges::Range<R> &&
                     std::convertible_to<Ranges::TRangeCommonReference<R>, FStringView>
        TOptional<U &> ResolveAsset(R &&Assets) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->AssetClasses.FindChecked(Key);
            auto Prefix = AssetClassData.AssetPrefix.Get(TEXT(""));
            // clang-format off
            auto FullNames = Assets |
                             Ranges::Map([&Prefix](ElementType AssetName) {
                                 return UAssetUtilities::GetFullAssetName(AssetName, Prefix);
                             });
            // clang-format on
            return UAssetLoader::ResolveAsset<U>(AssetClassData.RootDirectory, FullNames);
        }

        template <typename U = T, typename R>
            requires std::is_base_of_v<T, U> && Ranges::Range<R> && AssetKey<Ranges::TRangeCommonReference<R>>
        TArray<TOptional<U &>> LoadAssets(R &&Assets) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            // clang-format off
            return Assets |
                   Ranges::Map([this](ElementType Value) { return LoadAsset<U>(Value); }) |
                   Ranges::ToArray;
            // clang-format on
        }

      private:
        void OnPostEngineInit() {
            auto Settings = GetMutableDefault<UAssetLoadingSettings>();
            if (Settings->AssetClasses.Contains(Key)) {
                return;
            }

            Settings->AssetClasses.Emplace(Key, FAssetLoadingEntry(DefaultAssetPath, DefaultPrefix, T::StaticClass()));
        }

        FName Key;
        FDirectoryPath DefaultAssetPath;
        FStringView DefaultPrefix;
    };

    template <typename T>
        requires std::is_base_of_v<UObject, T>
    class TBlueprintClass {
      public:
        /**
         * Construct a new asset object using this loader
         * @param Key The key for the asset in question
         * @param DefaultPath The default path to the asset, should be a valid game directory
         * @param DefaultPrefix The default prefix for the asset type
         */
        explicit TBlueprintClass(FName Key, FStringView DefaultPath, FStringView DefaultPrefix = TEXT(""))
            : Key(Key), DefaultAssetPath(FString(DefaultPath)), DefaultPrefix(DefaultPrefix) {
            FCoreDelegates::OnPostEngineInit.AddRaw(this, &TBlueprintClass::OnPostEngineInit);
        }

        /**
         * Load the asset with the specified name
         * @param ClassName The name of the asset in question
         * @return The loaded asset
         */
        template <typename U = T>
            requires std::is_base_of_v<T, U>
        TOptional<TNonNullSubclassOf<U>> LoadClass(FStringView ClassName) const {
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->AssetClasses.FindChecked(Key);
            auto FullName = UAssetUtilities::GetFullAssetName(ClassName, AssetClassData.AssetPrefix.Get(TEXT("")));
            return UAssetLoader::LookupBlueprintClassByName<U>(AssetClassData.RootDirectory, FullName);
        }

        /**
         * Load the asset with the specified name
         * @param ClassName The name of the asset in question
         * @return The loaded asset
         */
        template <typename U = T>
            requires std::is_base_of_v<T, U>
        TOptional<TNonNullSubclassOf<U>> LoadClass(FName ClassName) const {
            if (ClassName.IsNone()) {
                return TOptional<TNonNullSubclassOf<U>>();
            }
            return LoadClass<U>(ClassName.ToString());
        }

        template <typename U = T, typename R>
            requires std::is_base_of_v<T, U> && Ranges::Range<R> &&
                     std::convertible_to<Ranges::TRangeCommonReference<R>, FStringView>
        TOptional<TNonNullSubclassOf<U>> ResolveClass(R &&Assets) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->BlueprintClasses.FindChecked(Key);
            auto Prefix = AssetClassData.AssetPrefix.Get(TEXT(""));
            // clang-format off
            auto FullNames = Assets |
                             Ranges::Map([&Prefix](ElementType AssetName) {
                                 return UAssetUtilities::GetFullAssetName(AssetName, Prefix);
                             });
            // clang-format on
            return UAssetLoader::ResolveClass<U>(AssetClassData.RootDirectory, FullNames);
        }

        template <typename U = T, typename R>
            requires std::is_base_of_v<U, T> && Ranges::Range<R> && AssetKey<Ranges::TRangeCommonReference<R>>
        TArray<TOptional<TNonNullSubclassOf<U>>> LoadClasses(R &&Classes) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            // clang-format off
            return Classes |
                   Ranges::Map([this](ElementType Value) { return LoadClass<U>(Value); }) |
                   Ranges::ToArray;
            // clang-format on
        }

      private:
        void OnPostEngineInit() {
            auto Settings = GetMutableDefault<UAssetLoadingSettings>();
            if (Settings->BlueprintClasses.Contains(Key)) {
                return;
            }

            Settings->BlueprintClasses.Emplace(Key,
                                               FAssetLoadingEntry(DefaultAssetPath, DefaultPrefix, T::StaticClass()));
        }

        FName Key;
        FDirectoryPath DefaultAssetPath;
        FStringView DefaultPrefix;
    };
} // namespace UE::Assets

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_ASSET_CLASS(Name, AssetType) const UE::Assets::TAssetClass<AssetType> Name

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_ASSET_CLASS_EXTERN(Name, AssetType) extern UE_DECLARE_ASSET_CLASS(Name, AssetType)

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_ASSET_CLASS_EXPORTED(ExportSymbol, Name, AssetType)                                                 \
    ExportSymbol UE_DECLARE_ASSET_CLASS_EXTERN(Name, AssetType)

/**
 * Define an asset class to use for loading assets.
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 * @param Directory The default directory for the asset class
 * @param Prefix The default prefix for the asset
 */
#define UE_DEFINE_ASSET_CLASS(Name, AssetType, Directory, Prefix)                                                      \
    const UE::Assets::TAssetClass<AssetType> Name(#Name, TEXT(Directory), TEXT(Prefix))

/**
 * Define an asset class to use for loading assets.
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 * @param Directory The default directory for the asset class
 * @param Prefix The default prefix for the asset
 */
#define UE_DEFINE_ASSET_CLASS_STATIC(Name, AssetType, Directory, Prefix)                                               \
    static UE_DEFINE_ASSET_CLASS(Name, AssetType, Directory, Prefix)

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_BLUEPRINT_CLASS(Name, AssetType) const UE::Assets::TBlueprintClass<AssetType> Name

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_BLUEPRINT_CLASS_EXTERN(Name, AssetType) extern UE_DECLARE_BLUEPRINT_CLASS(Name, AssetType)

/**
 * Declare an asset class with the following variable name and asset type
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 */
#define UE_DECLARE_BLUEPRINT_CLASS_EXPORTED(ExportSymbol, Name, AssetType)                                             \
    ExportSymbol UE_DECLARE_BLUEPRINT_CLASS_EXTERN(Name, AssetType)

/**
 * Define an asset class to use for loading assets.
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 * @param Directory The default directory for the asset class
 * @param Prefix The default prefix for the asset
 */
#define UE_DEFINE_BLUEPRINT_CLASS(Name, AssetType, Directory, Prefix)                                                  \
    const UE::Assets::TBlueprintClass<AssetType> Name(#Name, TEXT(Directory), TEXT(Prefix))

/**
 * Define an asset class to use for loading assets.
 * @param Name The name of the asset class
 * @param AssetType The type of the asset
 * @param Directory The default directory for the asset class
 * @param Prefix The default prefix for the asset
 */
#define UE_DEFINE_BLUEPRINT_CLASS_STATIC(Name, AssetType, Directory, Prefix)                                           \
    static UE_DEFINE_BLUEPRINT_CLASS(Name, AssetType, Directory, Prefix)