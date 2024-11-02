// "Unreal Pokémon" created by Retro & Chill.


#include "Services/GameServiceSubsystem.h"
#include "DependencyInjectionSettings.h"
#include "Engine/ObjectLibrary.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/Map.h"
#include "Services/Service.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Join.h"

void UGameServiceSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    for (TObjectIterator<UClass> It; It; ++It) {
        if (!It->IsChildOf<UService>()) {
            continue;
        }

        Services.Emplace(*It, NewObject<UService>(this, *It));
    }

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
    AssetData | UE::Ranges::Map(&FAssetData::GetAsset, TSet<FName>()) |
        UE::Ranges::Filter([](const UObject *Object) {
            return Object->IsA<UBlueprint>();
        }) |
        UE::Ranges::CastType<UBlueprint> |
        UE::Ranges::Map(&UBlueprint::GeneratedClass) |
        UE::Ranges::ForEach([this](UClass *Class) {
            Services.Emplace(Class, NewObject<UService>(this, Class));
        });
    // clang-format on
}

void UGameServiceSubsystem::Deinitialize() {
    Super::Deinitialize();
    Services.Empty();
}

UService *UGameServiceSubsystem::GetService(const TSubclassOf<UService> &ServiceClass) {
    // clang-format off
    return UE::Optionals::OfNullable(Services.Find(ServiceClass)) |
           UE::Optionals::Map(&TObjectPtr<UService>::Get) |
           UE::Optionals::GetPtrOrNull;
    // clang-format on
}