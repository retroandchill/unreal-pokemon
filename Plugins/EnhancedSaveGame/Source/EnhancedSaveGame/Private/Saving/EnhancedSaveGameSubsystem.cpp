// "Unreal Pokémon" created by Retro & Chill.

#include "Saving/EnhancedSaveGameSubsystem.h"
#include "EnhancedSaveGameModule.h"
#include "Kismet/GameplayStatics.h"
#include "RetroLib/Casting/UClassCasts.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "Saving/SaveableSubsystem.h"
#include "Saving/Serialization/EnhancedSaveGame.h"

#if WITH_UNREAL_INJECTOR
#include "Services/GameServiceSubsystem.h"
#endif

UEnhancedSaveGameSubsystem &UEnhancedSaveGameSubsystem::Get(const UObject *WorldContext) {
    // clang-format off
    return Retro::Optionals::OfNullable(WorldContext) |
           Retro::Optionals::Transform([](const UObject& O) { return UGameplayStatics::GetGameInstance(&O); }) |
           Retro::Optionals::Transform([](const UGameInstance &G) {
               return G.GetSubsystem<UEnhancedSaveGameSubsystem>();
           }) |
           Retro::Optionals::Value;
    // clang-format on
}

UEnhancedSaveGame *UEnhancedSaveGameSubsystem::CreateSaveGame(const FGameplayTagContainer &SaveTags) const {
    UE_LOG(LogEnhancedSaveGame, Log, TEXT("Generating the save game object"));
    auto SaveGame = NewObject<UEnhancedSaveGame>(GetGameInstance());
    auto Subsystems = GetGameInstance()->GetSubsystemArrayCopy<UGameInstanceSubsystem>();
    // clang-format off
    Subsystems |
        Retro::Ranges::Views::Filter(Retro::InstanceOf<ISaveableSubsystem>) |
        Retro::Ranges::ForEach(Retro::BindBack<&ISaveableSubsystem::Execute_CreateSaveData>(SaveGame, std::ref(SaveTags)));
    // clang-format on

#if WITH_UNREAL_INJECTOR
    auto ServiceSubsystem = GetGameInstance()->GetSubsystem<UGameServiceSubsystem>();
    check(ServiceSubsystem != nullptr)
    // clang-format off
    ServiceSubsystem->GetServicesOfType<ISaveableSubsystem>() |
        Retro::Ranges::Views::Transform(&FScriptInterface::GetObject) |
        Retro::Ranges::ForEach(Retro::BindBack<&ISaveableSubsystem::Execute_CreateSaveData>(SaveGame, std::ref(SaveTags)));
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
        Retro::Ranges::Views::Filter(Retro::InstanceOf<ISaveableSubsystem>) |
        Retro::Ranges::ForEach(Retro::BindBack<&ISaveableSubsystem::Execute_LoadSaveData>(SaveGame, std::ref(LoadTags)));
    // clang-format on

#if WITH_UNREAL_INJECTOR
    auto ServiceSubsystem = GetGameInstance()->GetSubsystem<UGameServiceSubsystem>();
    check(ServiceSubsystem != nullptr)
    // clang-format off
    ServiceSubsystem->GetServicesOfType<ISaveableSubsystem>() |
        Retro::Ranges::Views::Transform(&FScriptInterface::GetObject) |
        Retro::Ranges::ForEach(Retro::BindBack<&ISaveableSubsystem::Execute_LoadSaveData>(SaveGame, std::ref(LoadTags)));
    // clang-format on
#endif
}