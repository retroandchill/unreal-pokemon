// "Unreal Pokémon" created by Retro & Chill.

#include "Managers/PokemonSubsystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Lookup/InjectionUtilities.h"
#include "Player/Bag.h"
#include "Player/PlayerMetadata.h"
#include "Player/PlayerResetLocation.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Pokemon/Pokemon.h"
#include "Saving/PokemonSaveGame.h"
#include "Settings/PokemonStorageSystemSettings.h"
#include "Storage/StorageSystem.h"

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    GrowthRates.Empty();
    for (auto RegisteredTypes = Exp::FGrowthRateRegistry::GetInstance().GetAllRegisteredTypes();
         auto Type : RegisteredTypes) {
        GrowthRates.Add(Type, Exp::FGrowthRateRegistry::GetInstance().Construct(Type));
    }

    // TODO: Remove this and create a proper title screen
#if !UE_BUILD_SHIPPING
    StartNewGame();
#endif
}

UPokemonSubsystem &UPokemonSubsystem::GetInstance(const UObject *WorldContext) {
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
    check(GameInstance != nullptr)
    return *GameInstance->GetSubsystem<UPokemonSubsystem>();
}

bool UPokemonSubsystem::Exists(const UObject *WorldContext) {
    auto GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
    if (GameInstance == nullptr) {
        return false;
    }

    return GameInstance->GetSubsystem<UPokemonSubsystem>() != nullptr;
}

void UPokemonSubsystem::StartNewGame() {
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

const TScriptInterface<ITrainer> &UPokemonSubsystem::GetPlayer() const {
    return Player;
}

const TScriptInterface<IBag> &UPokemonSubsystem::GetBag() const {
    return Bag;
}

const TScriptInterface<IStorageSystem> &UPokemonSubsystem::GetStorageSystem() const {
    return StorageSystem;
}

UPlayerMetadata *UPokemonSubsystem::GetPlayerMetadata() const {
    return PlayerMetadata;
}

const Exp::IGrowthRate &UPokemonSubsystem::GetGrowthRate(FName GrowthRate) const {
    check(GrowthRates.Contains(GrowthRate))
    return *GrowthRates[GrowthRate];
}

FText UPokemonSubsystem::GetCurrentLocation() const {
    return CurrentLocation;
}

void UPokemonSubsystem::SetCurrentLocation(const FText &LocationName) {
    CurrentLocation = LocationName;
}

UPokemonSaveGame *UPokemonSubsystem::CreateSaveGame(TSubclassOf<UPokemonSaveGame> SaveGameClass) const {
    if (SaveGameClass == nullptr) {
        SaveGameClass = UPokemonSaveGame::StaticClass();
    }

    auto SaveGame = NewObject<UPokemonSaveGame>(SaveGameClass);
    SaveGame->PlayerCharacter = Player->ToDTO();
    SaveGame->Bag = Bag->ToDTO();
    SaveGame->StorageSystem = StorageSystem->ToDTO();

    SaveGame->CurrentMap = GetWorld()->GetMapName();
    auto PlayerCharacter = GetGameInstance()->GetPrimaryPlayerController(false)->GetCharacter();
    check(PlayerCharacter != nullptr)
    SaveGame->PlayerLocation = PlayerCharacter->GetActorTransform();

    check(PlayerResetLocation.IsSet())
    SaveGame->ResetMap = PlayerResetLocation->GetMapName();
    SaveGame->ResetLocation = PlayerResetLocation->GetPlayerTransform();

    SaveGame->StartDate = PlayerMetadata->StartDate;
    SaveGame->TotalPlaytime = PlayerMetadata->TotalPlaytime;
    SaveGame->RepelSteps = PlayerMetadata->RepelSteps;

    SaveGame->SaveDate = FDateTime::Now();
    return SaveGame;
}

void UPokemonSubsystem::LoadSave(UPokemonSaveGame *SaveGame, bool bChangeMap) {
    Player = UnrealInjector::NewInjectedDependency<ITrainer>(this, SaveGame->PlayerCharacter);
    Bag = UnrealInjector::NewInjectedDependency<IBag>(this, SaveGame->Bag);
    StorageSystem = UnrealInjector::NewInjectedDependency<IStorageSystem>(this, SaveGame->StorageSystem);
    PlayerMetadata->StartDate = SaveGame->StartDate;
    PlayerMetadata->TotalPlaytime = SaveGame->TotalPlaytime;
    PlayerMetadata->RepelSteps = SaveGame->RepelSteps;
    PlayerResetLocation.Emplace(SaveGame->ResetMap, SaveGame->ResetLocation);

    if (!bChangeMap) {
        return;
    }

    LoadTransform.Emplace(SaveGame->PlayerLocation);
    UGameplayStatics::OpenLevel(this, FName(*SaveGame->CurrentMap));
}

void UPokemonSubsystem::AdjustPlayerTransformOnLoad(ACharacter *PlayerCharacter) {
    if (LoadTransform.IsSet()) {
        PlayerCharacter->SetActorTransform(*LoadTransform);
        LoadTransform.Reset();
    }
}

bool UPokemonSubsystem::IsResetLocationSet() const {
    return PlayerResetLocation.IsSet();
}

void UPokemonSubsystem::PerformPlayerReset() {
    Player->HealParty();
    check(PlayerResetLocation.IsSet())
    LoadTransform.Emplace(PlayerResetLocation->GetPlayerTransform());
    UGameplayStatics::OpenLevel(this, FName(*PlayerResetLocation->GetMapName()));
}

void UPokemonSubsystem::SetPlayerResetLocation(const FString &MapName, const FTransform &Transform) {
    PlayerResetLocation.Emplace(MapName, Transform);
}

void UPokemonSubsystem::SetPlayerResetLocationAsCurrentLocation(ACharacter *PlayerCharacter) {
    SetPlayerResetLocation(GetWorld()->GetMapName(), PlayerCharacter->GetActorTransform());
}
