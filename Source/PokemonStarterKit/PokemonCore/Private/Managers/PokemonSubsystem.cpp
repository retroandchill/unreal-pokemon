// "Unreal Pokémon" created by Retro & Chill.

#include "Managers/PokemonSubsystem.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Lookup/InjectionUtilities.h"
#include "Player/Bag.h"
#include "Player/PlayerMetadata.h"
#include "Player/PlayerResetLocation.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Pokemon/Pokemon.h"
#include "Saving/PokemonSaveGame.h"
#include "Saving/Serialization/EnhancedSaveGame.h"
#include "Settings/PokemonStorageSystemSettings.h"
#include "Storage/StorageSystem.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    CoroutineDispatcher = UnrealInjector::NewInjectedDependency<IPokemonCoroutineDispatcher>(this);

    GrowthRates.Empty();
    for (auto RegisteredTypes = Exp::FGrowthRateRegistry::GetInstance().GetAllRegisteredTypes();
         auto Type : RegisteredTypes)
    {
        GrowthRates.Add(Type, Exp::FGrowthRateRegistry::GetInstance().Construct(Type));
    }

    // TODO: Remove this and create a proper title screen
#if !UE_BUILD_SHIPPING
    StartNewGame();
#endif
}

UPokemonSubsystem &UPokemonSubsystem::GetInstance(const UObject *WorldContext)
{
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
    check(GameInstance != nullptr)
    return *GameInstance->GetSubsystem<UPokemonSubsystem>();
}

bool UPokemonSubsystem::Exists(const UObject *WorldContext)
{
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
    if (GameInstance == nullptr)
    {
        return false;
    }

    return GameInstance->GetSubsystem<UPokemonSubsystem>() != nullptr;
}

void UPokemonSubsystem::StartNewGame()
{
    // TODO: Swap this instantiation with the actual trainer instantiation
    Player = UnrealInjector::NewInjectedDependency<ITrainer>(this, TEXT("POKEMONTRAINER_Nate"),
                                                             FText::FromStringView(TEXT("Nate")));
    Bag = UnrealInjector::NewInjectedDependency<IBag>(this);

    auto StorageSettings = GetDefault<UPokemonStorageSystemSettings>();
    StorageSystem = UnrealInjector::NewInjectedDependency<IStorageSystem>(this, StorageSettings->TotalBoxes,
                                                                          StorageSettings->BoxCapacity);

    PlayerMetadata = NewObject<UPlayerMetadata>();
    PlayerMetadata->StartNewGame();
}

const TScriptInterface<ITrainer> &UPokemonSubsystem::GetPlayer() const
{
    return Player;
}

const TScriptInterface<IBag> &UPokemonSubsystem::GetBag() const
{
    return Bag;
}

const TScriptInterface<IStorageSystem> &UPokemonSubsystem::GetStorageSystem() const
{
    return StorageSystem;
}

UPlayerMetadata *UPokemonSubsystem::GetPlayerMetadata() const
{
    return PlayerMetadata;
}

const Exp::IGrowthRate &UPokemonSubsystem::GetGrowthRate(FName GrowthRate) const
{
    check(GrowthRates.Contains(GrowthRate))
    return *GrowthRates[GrowthRate];
}

FText UPokemonSubsystem::GetCurrentLocation() const
{
    return CurrentLocation;
}

void UPokemonSubsystem::SetCurrentLocation(const FText &LocationName)
{
    CurrentLocation = LocationName;
}

void UPokemonSubsystem::CreateSaveData_Implementation(UEnhancedSaveGame *SaveGame,
                                                      const FGameplayTagContainer &SaveTags) const
{
    auto SaveData = NewObject<UPokemonSaveGame>();
    SaveData->PlayerCharacter = Player->ToDTO();
    SaveData->Bag = Bag->ToDTO();
    SaveData->StorageSystem = StorageSystem->ToDTO();

    SaveData->CurrentMap = GetWorld()->GetMapName();
    auto PlayerCharacter = GetGameInstance()->GetPrimaryPlayerController(false)->GetCharacter();
    check(PlayerCharacter != nullptr)
    SaveData->PlayerLocation = PlayerCharacter->GetActorTransform();

    check(PlayerResetLocation.IsSet())
    SaveData->ResetMap = PlayerResetLocation->GetMapName();
    SaveData->ResetLocation = PlayerResetLocation->GetPlayerTransform();

    SaveData->StartDate = PlayerMetadata->StartDate;
    SaveData->TotalPlaytime = PlayerMetadata->TotalPlaytime;
    SaveData->RepelSteps = PlayerMetadata->RepelSteps;

    SaveData->SaveDate = FDateTime::Now();
    SaveGame->AddObjectToSaveGame(Pokemon::Saving::PokemonCoreSaveData, SaveData);
}

void UPokemonSubsystem::LoadSaveData_Implementation(const UEnhancedSaveGame *SaveGame,
                                                    const FGameplayTagContainer &LoadTags)
{
    auto SaveData = SaveGame->LoadObjectFromSaveGame<UPokemonSaveGame>(Pokemon::Saving::PokemonCoreSaveData);
    Player = UnrealInjector::NewInjectedDependency<ITrainer>(this, SaveData->PlayerCharacter);
    Bag = UnrealInjector::NewInjectedDependency<IBag>(this, SaveData->Bag);
    StorageSystem = UnrealInjector::NewInjectedDependency<IStorageSystem>(this, SaveData->StorageSystem);
    PlayerMetadata->StartDate = SaveData->StartDate;
    PlayerMetadata->TotalPlaytime = SaveData->TotalPlaytime;
    PlayerMetadata->RepelSteps = SaveData->RepelSteps;
    PlayerResetLocation.Emplace(SaveData->ResetMap, SaveData->ResetLocation);

    if (!LoadTags.HasTag(Pokemon::Saving::ChangeMapOnLoad))
    {
        return;
    }

    LoadTransform.Emplace(SaveData->PlayerLocation);
    UGameplayStatics::OpenLevel(this, FName(*SaveData->CurrentMap));
}

void UPokemonSubsystem::AdjustPlayerTransformOnLoad(ACharacter *PlayerCharacter)
{
    if (LoadTransform.IsSet())
    {
        PlayerCharacter->SetActorTransform(*LoadTransform);
        LoadTransform.Reset();
    }
}

bool UPokemonSubsystem::IsResetLocationSet() const
{
    return PlayerResetLocation.IsSet();
}

void UPokemonSubsystem::PerformPlayerReset()
{
    Player->HealParty();
    check(PlayerResetLocation.IsSet())
    LoadTransform.Emplace(PlayerResetLocation->GetPlayerTransform());
    UGameplayStatics::OpenLevel(this, FName(*PlayerResetLocation->GetMapName()));
}

void UPokemonSubsystem::SetPlayerResetLocation(const FString &MapName, const FTransform &Transform)
{
    PlayerResetLocation.Emplace(MapName, Transform);
}

void UPokemonSubsystem::SetPlayerResetLocationAsCurrentLocation(ACharacter *PlayerCharacter)
{
    SetPlayerResetLocation(GetWorld()->GetMapName(), PlayerCharacter->GetActorTransform());
}
