// "Unreal Pokémon" created by Retro & Chill.

#include "Managers/PokemonSubsystem.h"
#include "Player/PlayerMetadata.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Settings/DependencyInjectionSettings.h"
#include "Settings/PokemonSettings.h"
#include "Settings/TrainerSettings.h"
#include "Trainers/TrainerStub.h"

UPokemonSubsystem *UPokemonSubsystem::Instance = nullptr;

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    Instance = this;
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

void UPokemonSubsystem::Deinitialize() {
    Super::Deinitialize();
    Instance = nullptr;
}

UPokemonSubsystem &UPokemonSubsystem::GetInstance() {
    check(Instance != nullptr)
    return *Instance;
}

bool UPokemonSubsystem::Exists() {
    return Instance != nullptr;
}

void UPokemonSubsystem::StartNewGame() {
    // TODO: Swap this instantiation with the actual trainer instantiation
    Player =
        NewObject<UTrainerStub>(this)->Initialize(TEXT("POKEMONTRAINER_Nate"), FText::FromStringView(TEXT("Nate")));
    Bag = NewObject<UObject>(this, GetDefault<UDependencyInjectionSettings>()->GetBagClass());
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

const TScriptInterface<IBag> & UPokemonSubsystem::GetBag() const {
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
