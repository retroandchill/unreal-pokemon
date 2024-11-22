// "Unreal Pokémon" created by Retro & Chill.

#include "Saving/EnhancedSaveGameSubsystem.h"
#include "EnhancedSaveGameModule.h"
#include "Kismet/GameplayStatics.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Algorithm/ToMap.h"
#include "Ranges/Casting/InstanceOf.h"
#include "Ranges/Optional/GetValue.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Pointers/SoftObjectRef.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Saving/SaveableSubsystem.h"
#include "Saving/Serialization/EnhancedSaveGame.h"

#if WITH_UNREAL_INJECTOR
#include "Services/GameServiceSubsystem.h"
#endif

UEnhancedSaveGameSubsystem &UEnhancedSaveGameSubsystem::Get(const UObject *WorldContext) {
    // clang-format off
    return UE::Optionals::OfNullable(WorldContext) |
           UE::Optionals::Map(&UGameplayStatics::GetGameInstance) |
           UE::Optionals::Map([](const UGameInstance &G) {
               return G.GetSubsystem<UEnhancedSaveGameSubsystem>();
           }) |
           UE::Optionals::GetValue;
    // clang-format on
}

UEnhancedSaveGame *UEnhancedSaveGameSubsystem::CreateSaveGame(const FGameplayTagContainer &SaveTags) const {
    UE_LOG(LogEnhancedSaveGame, Log, TEXT("Generating the save game object"));
    auto SaveGame = NewObject<UEnhancedSaveGame>(GetGameInstance());
    auto Subsystems = GetGameInstance()->GetSubsystemArrayCopy<UGameInstanceSubsystem>();
    // clang-format off
    Subsystems |
        UE::Ranges::Filter(UE::Ranges::InstanceOf<ISaveableSubsystem>) |
        UE::Ranges::ForEach(&ISaveableSubsystem::Execute_CreateSaveData, SaveGame, SaveTags);
    // clang-format on

#if WITH_UNREAL_INJECTOR
    auto ServiceSubsystem = GetGameInstance()->GetSubsystem<UGameServiceSubsystem>();
    check(ServiceSubsystem != nullptr)
    // clang-format off
    ServiceSubsystem->GetServicesOfType<ISaveableSubsystem>() |
        UE::Ranges::Map(&ISaveableSubsystem::_getUObject) |
        UE::Ranges::ForEach(&ISaveableSubsystem::Execute_CreateSaveData, SaveGame, SaveTags);
    // clang-format on
#endif

    UE_LOG(LogEnhancedSaveGame, Display, TEXT("Object creation complete"));
    return SaveGame;
}

void UEnhancedSaveGameSubsystem::LoadSaveGame(const UEnhancedSaveGame *SaveGame,
                                              const FGameplayTagContainer &LoadTags) const {
    auto Subsystems = GetGameInstance()->GetSubsystemArrayCopy<UGameInstanceSubsystem>();
    // clang-format off
    Subsystems |
        UE::Ranges::Filter(UE::Ranges::InstanceOf<ISaveableSubsystem>) |
        UE::Ranges::ForEach(&ISaveableSubsystem::Execute_LoadSaveData, SaveGame, LoadTags);
    // clang-format on

#if WITH_UNREAL_INJECTOR
    auto ServiceSubsystem = GetGameInstance()->GetSubsystem<UGameServiceSubsystem>();
    check(ServiceSubsystem != nullptr)
    // clang-format off
    ServiceSubsystem->GetServicesOfType<ISaveableSubsystem>() |
        UE::Ranges::Map(&ISaveableSubsystem::_getUObject) |
        UE::Ranges::ForEach(&ISaveableSubsystem::Execute_LoadSaveData, SaveGame, LoadTags);
    // clang-format on
#endif
}