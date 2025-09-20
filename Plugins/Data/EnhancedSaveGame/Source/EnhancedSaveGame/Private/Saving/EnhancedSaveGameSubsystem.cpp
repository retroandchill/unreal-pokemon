// "Unreal Pokémon" created by Retro & Chill.

#include "Saving/EnhancedSaveGameSubsystem.h"
#include "EnhancedSaveGameModule.h"
#include "Kismet/GameplayStatics.h"
#include "OptionalPtr.h"
#include "RangeV3.h"
#include "Saving/SaveableSubsystem.h"
#include "Saving/Serialization/EnhancedSaveGame.h"

UEnhancedSaveGameSubsystem &UEnhancedSaveGameSubsystem::Get(const UObject *WorldContext)
{
    auto *Subsystem = TOptionalPtr(WorldContext)
                          .Map([](const UObject *O) { return UGameplayStatics::GetGameInstance(O); })
                          .Map([](const UGameInstance *G) { return G->GetSubsystem<UEnhancedSaveGameSubsystem>(); })
                          .Get();
    check(Subsystem != nullptr);
    return *Subsystem;
}

UEnhancedSaveGame *UEnhancedSaveGameSubsystem::CreateSaveGame(const FGameplayTagContainer &SaveTags) const
{
    UE_LOG(LogEnhancedSaveGame, Log, TEXT("Generating the save game object"));
    auto *SaveGame = NewObject<UEnhancedSaveGame>(GetGameInstance());
    for (auto Subsystems = GetGameInstance()->GetSubsystemArrayCopy<UGameInstanceSubsystem>();
         const auto *Subsystem :
         Subsystems | ranges::views::filter([](const USubsystem *S) { return S->Implements<USaveableSubsystem>(); }))
    {
        ISaveableSubsystem::Execute_CreateSaveData(Subsystem, SaveGame, SaveTags);
    }

    UE_LOG(LogEnhancedSaveGame, Display, TEXT("Object creation complete"));
    return SaveGame;
}

void UEnhancedSaveGameSubsystem::LoadSaveGame(const UEnhancedSaveGame *SaveGame,
                                              const FGameplayTagContainer &LoadTags) const
{
    for (auto Subsystems = GetGameInstance()->GetSubsystemArrayCopy<UGameInstanceSubsystem>();
         auto *Subsystem :
         Subsystems | ranges::views::filter([](const USubsystem *S) { return S->Implements<USaveableSubsystem>(); }))
    {
        ISaveableSubsystem::Execute_LoadSaveData(Subsystem, SaveGame, LoadTags);
    }
}