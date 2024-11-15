// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/AssetLoader.h"
#include "Assets/AssetClass.h"
#include "Assets/AssetLoadingSettings.h"
#include "Assets/AssetUtilities.h"
#include "Ranges/Blueprints/BlueprintRuntimeUtils.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/OrElseGet.h"
#include "Ranges/String/CommonString.h"

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
                 UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::LookupAssetByName(const UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                                 const FString &AssetName, TSoftObjectPtr<> &FoundAsset) {
    // clang-format off
    FoundAsset = LookupAssetByName(BasePackageName, AssetName) |
                 UE::Optionals::Filter(&TSoftObjectRef<>::IsAssetOfType, AssetClass) |
                 UE::Optionals::Map(&TSoftObjectRef<>::ToSoftObjectPtr) |
                 UE::Optionals::OrElseGet([] { return TSoftObjectPtr(); });
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
            throw UE::Ranges::FInvalidArgumentException(
                EBlueprintExceptionType::AccessViolation,
                NSLOCTEXT("UAssetLoader", "LoadDynamicAsset_MissingParam", "The out parameter was not provided!"));
        }

        auto Registration = UE::Assets::FAssetClassRegistry::Get().GetAssetClassRegistration(Identifier);
        if (!Registration.IsSet()) {
            throw UE::Ranges::FInvalidArgumentException(EBlueprintExceptionType::AccessViolation,
                                                        NSLOCTEXT("UAssetLoader", "LoadDynamicAsset_NoAssetType",
                                                                  "The provided asset type is not a valid asset"));
        }

        auto AssetNameString = UE::Ranges::ExtractCommonStringFromProperty(AssetNameProp, AssetNameData);
        auto AssetName = UE::Ranges::GetStringView(AssetNameString);

        P_NATIVE_BEGIN
        P_GET_RESULT(EAssetLoadResult, Result);
        Result = Registration->LoadAsset(AssetName, *OutputProp, Output) ? EAssetLoadResult::Found
                                                                         : EAssetLoadResult::NotFound;
        P_NATIVE_END;
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
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
            throw UE::Ranges::FInvalidArgumentException(
                EBlueprintExceptionType::AccessViolation,
                NSLOCTEXT("UAssetLoader", "LookupDynamicAsset_MissingParam", "The out parameter was not provided!"));
        }

        auto Registration = UE::Assets::FAssetClassRegistry::Get().GetAssetClassRegistration(Identifier);
        if (!Registration.IsSet()) {
            throw UE::Ranges::FInvalidArgumentException(EBlueprintExceptionType::AccessViolation,
                                                        NSLOCTEXT("UAssetLoader", "LookupDynamicAsset_NoAssetType",
                                                                  "The provided asset type is not a valid asset"));
        }

        auto AssetNameString = UE::Ranges::ExtractCommonStringFromProperty(AssetNameProp, AssetNameData);
        auto AssetName = UE::Ranges::GetStringView(AssetNameString);

        P_NATIVE_BEGIN
        P_GET_RESULT(EAssetLoadResult, Result);
        Result = Registration->LookupAsset(AssetName, *OutputProp, Output) ? EAssetLoadResult::Found
                                                                           : EAssetLoadResult::NotFound;
        P_NATIVE_END;
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

EAssetLoadResult UAssetLoader::LookupBlueprintClassByName(UClass *BaseClass, const FDirectoryPath &BasePackageName,
                                                          const FString &AssetName, UClass *&FoundClass) {
    FoundClass = LookupBlueprintClassByName(BasePackageName, AssetName) |
                 UE::Optionals::Filter([&BaseClass](const UClass *Class) { return Class->IsChildOf(BaseClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundClass != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::ResolveAsset(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                            const TArray<FString> &Keys, UObject *&FoundAsset) {
    FoundAsset = ResolveAsset(BasePackageName, Keys) |
                 UE::Optionals::Filter([&AssetClass](const UObject &Object) { return Object.IsA(AssetClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundAsset != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}

EAssetLoadResult UAssetLoader::ResolveClass(UClass *AssetClass, const FDirectoryPath &BasePackageName,
                                            const TArray<FString> &Keys, UClass *&FoundClass) {
    FoundClass = ResolveClass(BasePackageName, Keys) |
                 UE::Optionals::Filter([&AssetClass](const UObject *Object) { return Object->IsA(AssetClass); }) |
                 UE::Optionals::GetPtrOrNull;
    return FoundClass != nullptr ? EAssetLoadResult::Found : EAssetLoadResult::NotFound;
}