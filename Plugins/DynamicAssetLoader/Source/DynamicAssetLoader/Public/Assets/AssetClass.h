// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AssetLoader.h"
#include "AssetLoadingSettings.h"
#include "AssetUtilities.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Variants/VariantObject.h"
#include "Ranges/Views/Map.h"

namespace UE::Assets {

    template <typename T>
    concept AssetKey = std::is_same_v<std::remove_cvref_t<T>, FName> || std::is_convertible_v<T, FStringView>;

    class FAssetClassRegistry;

    /**
     * Class used for native declarations of asset classes
     * @tparam T The class of the asset in question
     */
    template <typename T>
        requires AssetClassType<T>
    class TAssetClass {
    public:
        template <typename U>
        static constexpr bool ValidTemplateParam = (std::is_base_of_v<T, U> && std::is_base_of_v<UObject, T>) || (std::same_as<T, U> && Ranges::VariantObjectStruct<T>);

        const FName &GetKey() const {
            return Key;
        }

        /**
         * Construct a new asset object using this loader
         * @param Key The key for the asset in question
         * @param DefaultPath The default path to the asset, should be a valid game directory
         * @param DefaultPrefix The default prefix for the asset type
         */
        explicit TAssetClass(FName Key, FStringView DefaultPath, FStringView DefaultPrefix = TEXT("")) : Key(Key) {
            FCoreDelegates::OnPostEngineInit.AddRaw(this, &TAssetClass::OnPostEngineInit, DefaultPath, DefaultPrefix);
        }

        /**
         * Load the asset with the specified name
         * @param AssetName The name of the asset in question
         * @return The loaded asset
         */
        template <typename U = T>
            requires ValidTemplateParam<U>
        auto LoadAsset(FStringView AssetName) const {
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->AssetClasses.FindChecked(Key);
            auto FullName = UAssetUtilities::GetFullAssetName(AssetName, AssetClassData.AssetPrefix.Get(TEXT("")));
            if constexpr (std::is_base_of_v<UObject, T>) {
                return UAssetLoader::FindAssetByName<U>(AssetClassData.RootDirectory, FullName);
            } else {
                static_assert(Ranges::VariantObjectStruct<T>);
                return UAssetLoader::FindAssetByName(AssetClassData.RootDirectory, FullName) |
                       Optionals::Map([](UObject &Object) {
                           return T(&Object);
                       });
            }
        }

        /**
         * Load the asset with the specified name
         * @param AssetName The name of the asset in question
         * @return The loaded asset
         */
        template <typename U = T>
            requires ValidTemplateParam<U>
        auto LoadAsset(FName AssetName) const {
            return LoadAsset<U>(AssetName.ToString());
        }

        template <typename U = T>
            requires ValidTemplateParam<U>
        auto LookupAsset(FStringView AssetName) const {
            auto Settings = GetDefault<UAssetLoadingSettings>();
            auto AssetClassData = Settings->AssetClasses.FindChecked(Key);
            auto FullName = UAssetUtilities::GetFullAssetName(AssetName, AssetClassData.AssetPrefix.Get(TEXT("")));

            if constexpr (std::is_base_of_v<UObject, T>) {
                return UAssetLoader::LookupAssetByName<T>(AssetClassData.RootDirectory, FullName);
            } else {
                static_assert(Ranges::VariantObjectStruct<T>);
                return UAssetLoader::LookupAssetByName(AssetClassData.RootDirectory, FullName) |
                       Optionals::Map([](const TSoftObjectRef<> &Object) {
                           return T::SoftPtrType(Object.ToSoftObjectPtr());
                       });
            }
        }

        template <typename U = T>
            requires ValidTemplateParam<U>
        auto LookupAsset(FName AssetName) const {
            return LookupAsset<U>(AssetName.ToString());
        }

        template <typename U = T, typename R>
            requires ValidTemplateParam<U> && Ranges::Range<R> &&
                     std::convertible_to<Ranges::TRangeCommonReference<R>, FStringView>
        auto ResolveAsset(R &&Assets) const {
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
            if constexpr (std::is_base_of_v<UObject, T>) {
                return UAssetLoader::ResolveAsset<U>(AssetClassData.RootDirectory, FullNames);
            } else {
                static_assert(Ranges::VariantObjectStruct<T>);
                return UAssetLoader::ResolveAsset(AssetClassData.RootDirectory, FullNames) |
                       Optionals::Map([](UObject &Object) {
                           return T(&Object);
                       });
            }
        }

        template <typename U = T, typename R>
            requires ValidTemplateParam<U> && Ranges::Range<R> &&
                     std::convertible_to<Ranges::TRangeCommonReference<R>, FStringView>
        auto ResolveSoftAsset(R &&Assets) const {
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
            if constexpr (std::is_base_of_v<UObject, U>) {
                return UAssetLoader::ResolveSoftAsset<U>(AssetClassData.RootDirectory, FullNames);
            } else {
                static_assert(Ranges::VariantObjectStruct<T>);
                return UAssetLoader::ResolveSoftAsset(AssetClassData.RootDirectory, FullNames) |
                       Optionals::Map([](const TSoftObjectRef<> &Object) {
                           return T::SoftPtrType(Object.ToSoftObjectPtr());
                       });
            }
        }

        template <typename U = T, typename R>
            requires ValidTemplateParam<U> && Ranges::Range<R> && AssetKey<Ranges::TRangeCommonReference<R>>
        auto LoadAssets(R &&Assets) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            // clang-format off
            return Assets |
                   Ranges::Map([this](ElementType Value) {
                       return LoadAsset<U>(Value);
                   }) |
                   Ranges::ToArray;
            // clang-format on
        }

        template <typename U = T, typename R>
            requires ValidTemplateParam<U> && Ranges::Range<R> && AssetKey<Ranges::TRangeCommonReference<R>>
        auto LookupAssets(R &&Assets) const {
            using ElementType = Ranges::TRangeCommonReference<R>;
            // clang-format off
            return Assets |
                   Ranges::Map([this](ElementType Value) {
                       return LookupAsset<U>(Value);
                   }) |
                   Ranges::ToArray;
            // clang-format on
        }

    private:
        void OnPostEngineInit(FStringView DefaultAssetPath, FStringView DefaultPrefix) {
            auto Settings = GetMutableDefault<UAssetLoadingSettings>();
            if (Settings->AssetClasses.Contains(Key)) {
                return;
            }

            if constexpr (std::is_base_of_v<UObject, T>) {
                Settings->AssetClasses.Emplace(Key,
                                               FAssetLoadingEntry(DefaultAssetPath, DefaultPrefix, T::StaticClass()));
            } else if constexpr (Ranges::VariantObjectStruct<T>) {
                if constexpr (Ranges::CoreStructType<T>) {
                    Settings->AssetClasses.Emplace(
                        Key, FAssetLoadingEntry(DefaultAssetPath, DefaultPrefix, TBaseStructure<T>::Get()));
                } else {
                    Settings->AssetClasses.Emplace(
                        Key, FAssetLoadingEntry(DefaultAssetPath, DefaultPrefix, T::StaticStruct()));
                }
            }
        }

        FName Key;
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
        explicit TBlueprintClass(FName Key, FStringView DefaultPath, FStringView DefaultPrefix = TEXT("")) : Key(Key) {
            FCoreDelegates::OnPostEngineInit.AddRaw(this, &TBlueprintClass::OnPostEngineInit,
                                                    DefaultPath, DefaultPrefix);
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
            auto AssetClassData = Settings->BlueprintClasses.FindChecked(Key);
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
                   Ranges::Map([this](ElementType Value) {
                       return LoadClass<U>(Value);
                   }) |
                   Ranges::ToArray;
            // clang-format on
        }

    private:
        void OnPostEngineInit(FStringView DefaultAssetPath, FStringView DefaultPrefix) {
            auto Settings = GetMutableDefault<UAssetLoadingSettings>();
            if (Settings->BlueprintClasses.Contains(Key)) {
                return;
            }

            Settings->BlueprintClasses.Emplace(Key,
                                               FAssetLoadingEntry(DefaultAssetPath, DefaultPrefix, T::StaticClass()));
        }

        FName Key;
    };

    class IAssetClassRegistration {
    public:
        virtual ~IAssetClassRegistration() = default;

        virtual bool LoadAsset(FStringView AssetName, const FProperty &Property, uint8 *Data) const = 0;
        virtual bool LookupAsset(FStringView AssetName, const FProperty &Property, uint8 *Data) const = 0;
    };

    template <typename T>
        requires AssetClassType<T>
    class TAssetClassRegistrationImpl : public IAssetClassRegistration {
    public:
        explicit TAssetClassRegistrationImpl(const TAssetClass<T> &AssetClass) : AssetClass(AssetClass) {
        }

        bool LoadAsset(FStringView AssetName, const FProperty &Property, uint8 *Data) const override {
            auto Result = AssetClass.LoadAsset(AssetName);
            if (!Result.IsSet()) {
                return false;
            }

            if constexpr (Ranges::VariantObjectStruct<T>) {
                auto StructProperty = CastField<FStructProperty>(&Property);
                if (StructProperty == nullptr || StructProperty->Struct.Get() != Ranges::GetScriptStruct<T>()) {
                    throw Ranges::FTypeException(EBlueprintExceptionType::AccessViolation,
                                                 NSLOCTEXT("TAssetClassRegistrationImpl", "IncompatibleProperty",
                                                           "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
                }

                void* StructPtr = Data;
                auto &Struct = *static_cast<T*>(StructPtr);
                Struct = *Result;
            } else {
                static_assert(std::derived_from<T, UObject>);
                auto ObjectProperty = CastField<FObjectProperty>(&Property);
                if (ObjectProperty == nullptr || !ObjectProperty->PropertyClass->IsChildOf<T>()) {
                    throw Ranges::FTypeException(EBlueprintExceptionType::AccessViolation,
                                                 NSLOCTEXT("TAssetClassRegistrationImpl", "IncompatibleProperty_Object",
                                                           "Incompatible output parameter; the supplied object does is not of the correct type for this object."));
                }

                ObjectProperty->SetObjectPropertyValue(Data, Result.GetPtrOrNull());
            }

            return true;
        }
        
        bool LookupAsset(FStringView AssetName, const FProperty &Property, uint8 *Data) const override {
            auto Result = AssetClass.LookupAsset(AssetName);
            if (!Result.IsSet()) {
                return false;
            }

            if constexpr (Ranges::VariantObjectStruct<T>) {
                auto StructProperty = CastField<FStructProperty>(&Property);
                if (StructProperty == nullptr || StructProperty->Struct.Get() != Ranges::GetScriptStruct<typename T::SoftPtrType>()) {
                    throw Ranges::FTypeException(EBlueprintExceptionType::AccessViolation,
                                                 NSLOCTEXT("TAssetClassRegistrationImpl", "IncompatibleProperty",
                                                           "Incompatible output parameter; the supplied struct does not have the same layout as what is expected for a variant object struct."));
                }

                void* StructPtr = Data;
                auto &Struct = *static_cast<typename T::SoftPtrType*>(StructPtr);
                Struct = *Result;
            } else {
                static_assert(std::derived_from<T, UObject>);
                auto ObjectProperty = CastField<FSoftObjectProperty>(&Property);
                if (ObjectProperty == nullptr || ObjectProperty->PropertyClass->IsChildOf<T>()) {
                    throw Ranges::FTypeException(EBlueprintExceptionType::AccessViolation,
                                                 NSLOCTEXT("TAssetClassRegistrationImpl", "IncompatibleProperty_Object",
                                                           "Incompatible output parameter; the supplied object does is not of the correct type for this object."));
                }
                
                ObjectProperty->SetValue_InContainer(Data, FSoftObjectPtr(Result->ToSoftObjectPath()));
            }

            return true;
        }

    private:
        const TAssetClass<T> AssetClass;
    };

    class DYNAMICASSETLOADER_API FAssetClassRegistry {
        FAssetClassRegistry() = default;
        ~FAssetClassRegistry() = default;

    public:
        FAssetClassRegistry(const FAssetClassRegistry &) = delete;
        FAssetClassRegistry(FAssetClassRegistry &&) = delete;
        FAssetClassRegistry &operator=(const FAssetClassRegistry &) = delete;
        FAssetClassRegistry &operator=(FAssetClassRegistry &&) = delete;

        static FAssetClassRegistry &Get();

        template <typename T>
            requires AssetClassType<T>
        bool RegisterAssetClass(const TAssetClass<T> &Registration) {
            Ranges::AddToDelegate(FCoreDelegates::OnPostEngineInit, [this, &Registration] {
                auto Key = Registration.GetKey();
                if (AssetClassRegistry.Contains(Key)) {
                    return;
                }

                AssetClassRegistry.Emplace(Key, MakeUnique<TAssetClassRegistrationImpl<T>>(Registration));
            });
            
            return true;
        }

        TOptional<IAssetClassRegistration&> GetAssetClassRegistration(FName Key) const;

    private:
        TMap<FName, TUniquePtr<IAssetClassRegistration>> AssetClassRegistry;
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
    const UE::Assets::TAssetClass<AssetType> Name(#Name, TEXT(Directory), TEXT(Prefix)); \
    static const bool __AssetClassType_##Name##_Registered = UE::Assets::FAssetClassRegistry::Get().RegisterAssetClass(Name)

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