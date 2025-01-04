// "Unreal Pokémon" created by Retro & Chill.

#include "Services/ServiceUtilities.h"
#include "DependencyInjectionSettings.h"
#include "Engine/ObjectLibrary.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Casting/UClassCasts.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Compatibility/Array.h"
#include "RetroLib/Ranges/Views/ClassView.h"
#include "RetroLib/Ranges/Views/NameAliases.h"
#include "Services/Service.h"

Retro::TGenerator<TSubclassOf<UService>> UnrealInjector::GetAllServices() {
    TSet<UClass *> Visited;
    for (auto Class : Retro::Ranges::TClassView<UService>()) {
        if (Class->HasAnyClassFlags(CLASS_Abstract) || !Retro::IsInstantiableClass(Class)) {
            continue;
        }
        co_yield Class;
        Visited.Add(Class);
    }

    auto Settings = GetDefault<UDependencyInjectionSettings>();
    // clang-format off
    auto Paths = Settings->BlueprintServiceScan |
                 Retro::Ranges::Views::Transform<&FDirectoryPath::Path>() |
                 Retro::Ranges::To<TArray>();
    // clang-format on

    auto ObjectLibrary = UObjectLibrary::CreateLibrary(UService::StaticClass(), true, true);
    ObjectLibrary->LoadBlueprintsFromPaths(Paths);

    TArray<FAssetData> AssetData;
    ObjectLibrary->GetAssetDataList(AssetData);

    // clang-format off
    co_yield Retro::Ranges::TElementsOf(AssetData |
        Retro::Ranges::Views::Transform<&FAssetData::GetAsset>(TSet<FName>()) |
        Retro::Ranges::Views::Filter([](const UObject *Object) {
            return Object->IsA<UBlueprint>();
        }) |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<UBlueprint>) |
        Retro::Ranges::Views::Transform<&UBlueprint::GeneratedClass>() |
        Retro::Ranges::Views::Filter([&](const UClass* Class) { return !Visited.Contains(Class); }) |
        Retro::Ranges::Views::Filter([](const UClass* Class) { return !Class->HasAnyClassFlags(CLASS_Abstract); }) |
        Retro::Ranges::Views::Filter<&Retro::IsInstantiableClass>() |
        Retro::Ranges::Views::Transform([](UClass* Class) { return TSubclassOf<UService>(Class); }));
    // clang-format on
}