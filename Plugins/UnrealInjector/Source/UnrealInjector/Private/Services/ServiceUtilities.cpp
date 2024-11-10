// "Unreal Pokémon" created by Retro & Chill.

#include "Services/ServiceUtilities.h"
#include "DependencyInjectionSettings.h"
#include "Engine/ObjectLibrary.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/ClassView.h"
#include "Ranges/Views/Concat.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"
#include "Ranges/Views/SharedArrayView.h"
#include "Ranges/Views/Unique.h"
#include "Services/Service.h"

UE::Ranges::TAnyView<TSubclassOf<UService>> UnrealInjector::GetAllServices() {
    auto Settings = GetDefault<UDependencyInjectionSettings>();
    // clang-format off
    auto Paths = Settings->BlueprintServiceScan |
                 UE::Ranges::Map(&FDirectoryPath::Path) |
                 UE::Ranges::ToArray;
    // clang-format on

    auto ObjectLibrary = UObjectLibrary::CreateLibrary(UService::StaticClass(), true, true);
    ObjectLibrary->LoadBlueprintsFromPaths(Paths);

    TArray<FAssetData> AssetData;
    ObjectLibrary->GetAssetDataList(AssetData);

    // clang-format off
    auto FilteredAssetData = UE::Ranges::TSharedArrayView(std::move(AssetData)) |
        UE::Ranges::Map(&FAssetData::GetAsset, TSet<FName>()) |
        UE::Ranges::Filter([](const UObject *Object) {
            return Object->IsA<UBlueprint>();
        }) |
        UE::Ranges::CastType<UBlueprint> |
        UE::Ranges::Map(&UBlueprint::GeneratedClass) |
        UE::Ranges::Filter([](const UClass* Class) { return !Class->HasAnyClassFlags(CLASS_Abstract); }) |
        UE::Ranges::Filter(&UE::Ranges::IsInstantiableClass) |
        UE::Ranges::Map([](UClass* Class) { return TSubclassOf<UService>(Class); });
    // clang-format on

    return UE::Ranges::Concat(UE::Ranges::TClassView<UService>(), std::move(FilteredAssetData)) | UE::Ranges::Unique;
}