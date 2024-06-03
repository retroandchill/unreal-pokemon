// "Unreal Pokémon" created by Retro & Chill.

#include "Managers/PokemonSubsystem.h"
#include "Lookup/InjectionUtilities.h"
#include "Player/Bag.h"
#include "Player/PlayerMetadata.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Settings/PokemonSettings.h"
#include "Settings/TrainerSettings.h"
#include "Trainers/TrainerStub.h"

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    HPStat = GetDefault<UPokemonSettings>()->GetHPStat();
    MaxPartySize = GetDefault<UTrainerSettings>()->GetMaxPartySize();

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

FName UPokemonSubsystem::GetHPStat() const {
    return HPStat;
}

int32 UPokemonSubsystem::GetMaxPartySize() const {
    return MaxPartySize;
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
