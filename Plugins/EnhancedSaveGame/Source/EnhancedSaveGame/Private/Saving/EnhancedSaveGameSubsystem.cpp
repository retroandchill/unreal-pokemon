// "Unreal Pokémon" created by Retro & Chill.


#include "Saving/EnhancedSaveGameSubsystem.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Algorithm/ToMap.h"
#include "Saving/SaveableSubsystem.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/FilterImplements.h"
#include "Saving/Serialization/EnhancedSaveGame.h"

#if WITH_UNREAL_INJECTOR
#include "Services/GameServiceSubsystem.h"
#endif

UEnhancedSaveGame *UEnhancedSaveGameSubsystem::CreateSaveGame() const {
    auto SaveGame = NewObject<UEnhancedSaveGame>(GetGameInstance());
    auto &Subsystems = GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>();
    // clang-format off
    Subsystems | UE::Ranges::FilterImplements<ISaveableSubsystem> |
        UE::Ranges::ForEach(&ISaveableSubsystem::Execute_CreateSaveData, SaveGame);
    // clang-format on

#if WITH_UNREAL_INJECTOR
    auto ServiceSubsystem = GetGameInstance()->GetSubsystem<UGameServiceSubsystem>();
    check(ServiceSubsystem != nullptr);
    // clang-format off
    ServiceSubsystem->GetServicesOfType<ISaveableSubsystem>() |
        UE::Ranges::ForEach(&ISaveableSubsystem::Execute_CreateSaveData, SaveGame);
    // clang-format on
#endif

    return SaveGame;
}