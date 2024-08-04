// "Unreal Pokémon" created by Retro & Chill.

#include "Managers/PokemonSubsystem.h"
#include "GameFramework/Character.h"
#include "Lookup/InjectionUtilities.h"
#include "Player/Bag.h"
#include "Player/PlayerMetadata.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Saving/PokemonSaveGame.h"
#include "Trainers/TrainerStub.h"

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);
    Collection.InitializeDependency(UDependencyInjectionSubsystem::StaticClass());

    GrowthRates.Empty();
    for (auto RegisteredTypes = Exp::FGrowthRateRegistry::GetInstance().GetAllRegisteredTypes();
         auto Type : RegisteredTypes) {
        GrowthRates.Add(Type, Exp::FGrowthRateRegistry::GetInstance().Construct(Type));
    }

#if WITH_EDITOR
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
    Player =
        NewObject<UTrainerStub>(this)->Initialize(TEXT("POKEMONTRAINER_Nate"), FText::FromStringView(TEXT("Nate")));
    Bag = UnrealInjector::NewInjectedDependency<IBag>(this);

    // TODO: Remove this temp inventory stuff
#if WITH_EDITOR
    Bag->ObtainItem(TEXT("REPEL"), 50);
    Bag->ObtainItem(TEXT("POTION"), 20);
    Bag->ObtainItem(TEXT("ANTIDOTE"), 10);
    Bag->ObtainItem(TEXT("POKEBALL"), 10);
#endif

    PlayerMetadata = NewObject<UPlayerMetadata>();
    PlayerMetadata->StartNewGame();
}

const TScriptInterface<ITrainer> &UPokemonSubsystem::GetPlayer() const {
    return Player;
}

const TScriptInterface<IBag> &UPokemonSubsystem::GetBag() const {
    return Bag;
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

UPokemonSaveGame * UPokemonSubsystem::CreateSaveGame(TSubclassOf<UPokemonSaveGame> SaveGameClass) const {
    if (SaveGameClass == nullptr) {
        SaveGameClass = UPokemonSaveGame::StaticClass();
    }
    
    auto SaveGame = NewObject<UPokemonSaveGame>(SaveGameClass);
    SaveGame->PlayerCharacter = Player;
    SaveGame->Bag = Bag;
    SaveGame->PlayerMetadata = PlayerMetadata;

    SaveGame->CurrentMap = GetWorld()->GetMapName();
    auto PlayerCharacter = GetGameInstance()->GetPrimaryPlayerController(false)->GetCharacter();
    check(PlayerCharacter != nullptr)
    SaveGame->PlayerLocation = PlayerCharacter->GetActorTransform();

    return SaveGame;
}

void UPokemonSubsystem::LoadSave(UPokemonSaveGame *SaveGame, bool bChangeMap) {
    Player = SaveGame->PlayerCharacter;
    Bag = SaveGame->Bag;
    PlayerMetadata = SaveGame->PlayerMetadata;

    if (!bChangeMap) {
        return;
    }

    LoadTransform.Emplace(SaveGame->PlayerLocation);
    UGameplayStatics::OpenLevel(this, FName(*SaveGame->CurrentMap));
}

void UPokemonSubsystem::AdjustPlayerTransformOnLoad(ACharacter* PlayerCharacter) {
    if (LoadTransform.IsSet()) {
        PlayerCharacter->SetActorTransform(*LoadTransform);
        LoadTransform.Reset();
    }
}
