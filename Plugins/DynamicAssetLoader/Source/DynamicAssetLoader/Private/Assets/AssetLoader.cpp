// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/AssetLoader.h"
#include "Assets/AssetClass.h"
#include "RetroLib/Exceptions/ConvertException.h"
#include "RetroLib/Exceptions/InvalidArgumentException.h"
#include "RetroLib/Optionals/OrElseGet.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Utils/CommonString.h"

FString UAssetLoader::CreateSearchKey(FStringView BasePackageName, FStringView AssetName) {
    FStringView Prefix;
    if (int32 CharIndex; AssetName.FindLastChar('/', CharIndex)) {
        int32 PrefixLength = CharIndex + 1;
        Prefix = AssetName.SubStr(0, PrefixLength);
        AssetName = AssetName.RightChop(PrefixLength);
    }

    return FString::Format(TEXT("{0}/{1}{2}.{2}"), {BasePackageName, Prefix, AssetName});
}

EAssetLoadResult UAssetLoader::FindAssetByName(const UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                               const FString &AssetName, UObject *&FoundAsset) {
    FoundAsset = FindAssetByName(BasePackageName, AssetName) |
                 Retro::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
                 Retro::Optionals::PtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::LookupAssetByName(const UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                                 const FString &AssetName, TSoftObjectPtr<> &FoundAsset) {
    // clang-format off
    FoundAsset = LookupAssetByName(BasePackageName, AssetName) |
                 Retro::Optionals::Filter(Retro::BindBack<&TSoftObjectRef<>::IsAssetOfType>(AssetClass)) |
                 Retro::Optionals::OrElseGet([] { return TSoftObjectPtr(); });
    // clang-format on
    return !FoundAsset.IsNull() ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

CUSTOM_THUNK_STUB(EAssetLoadResult, UAssetLoader::LoadDynamicAsset, FName, const FString &, UObject *&)
DEFINE_FUNCTION(UAssetLoader::execLoadDynamicAsset) {
    P_GET_PROPERTY(FNameProperty, Identifier)
    P_GET_WILDCARD_PARAM(AssetNameProp, AssetNameData)
    P_GET_WILDCARD_PARAM(OutputProp, Output)
    P_FINISH

    try {
        if (OutputProp == nullptr || Output == nullptr) {
            throw Retro::FInvalidArgumentException("The out parameter was not provided!");
        }

        auto Registration = UE::Assets::FAssetClassRegistry::Get().GetAssetClassRegistration(Identifier);
        if (!Registration.IsSet()) {
            throw Retro::FInvalidArgumentException("The provided asset type is not a valid asset");
        }

        auto AssetNameString = Retro::ExtractCommonStringFromProperty(AssetNameProp, AssetNameData);
        auto AssetName = Retro::GetStringView(AssetNameString);

        P_NATIVE_BEGIN
        P_GET_RESULT(EAssetLoadResult, Result);
        Result = Registration->LoadAsset(AssetName, *OutputProp, Output) ? EAssetLoadResult::Found
                                                                         : EAssetLoadResult::NotFound;
        P_NATIVE_END;
    } catch (const Retro::FBlueprintException &Exception) {
        ConvertException(Exception);
    }
}

CUSTOM_THUNK_STUB(EAssetLoadResult, UAssetLoader::LookupDynamicAsset, FName, const FString &, TSoftObjectPtr<> &)
DEFINE_FUNCTION(UAssetLoader::execLookupDynamicAsset) {
    P_GET_PROPERTY(FNameProperty, Identifier)
    P_GET_WILDCARD_PARAM(AssetNameProp, AssetNameData)
    P_GET_WILDCARD_PARAM(OutputProp, Output)
    P_FINISH

    try {
        if (OutputProp == nullptr || Output == nullptr) {
            throw Retro::FInvalidArgumentException("The out parameter was not provided!");
        }

        auto Registration = UE::Assets::FAssetClassRegistry::Get().GetAssetClassRegistration(Identifier);
        if (!Registration.IsSet()) {
            throw Retro::FInvalidArgumentException("The provided asset type is not a valid asset");
        }

        auto AssetNameString = Retro::ExtractCommonStringFromProperty(AssetNameProp, AssetNameData);
        auto AssetName = Retro::GetStringView(AssetNameString);

        P_NATIVE_BEGIN
        P_GET_RESULT(EAssetLoadResult, Result);
        Result = Registration->LookupAsset(AssetName, *OutputProp, Output) ? EAssetLoadResult::Found
                                                                           : EAssetLoadResult::NotFound;
        P_NATIVE_END;
    } catch (const Retro::FBlueprintException &Exception) {
        ConvertException(Exception);
    }
}

EAssetLoadResult UAssetLoader::LookupBlueprintClassByName(UClass *BaseClass, const FDirectoryPath &BasePackageName,
                                                          const FString &AssetName, UClass *&FoundClass) {
    FoundClass = LookupBlueprintClassByName(BasePackageName, AssetName) |
                 Retro::Optionals::Filter([&BaseClass](const UClass *Class) { return Class->IsChildOf(BaseClass); }) |
                 Retro::Optionals::PtrOrNull;
    return FoundClass != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::ResolveAsset(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                            const TArray<FString> &Keys, UObject *&FoundAsset) {
    FoundAsset = ResolveAsset(BasePackageName, Keys) |
                 Retro::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
                 Retro::Optionals::PtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::ResolveClass(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                            const TArray<FString> &Keys, UClass *&FoundClass) {
    FoundClass = ResolveClass(BasePackageName, Keys) |
                 Retro::Optionals::Filter([&AssetClass](const UObject *Object) { return Object->IsA(AssetClass); }) |
                 Retro::Optionals::PtrOrNull;
    return FoundClass != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}