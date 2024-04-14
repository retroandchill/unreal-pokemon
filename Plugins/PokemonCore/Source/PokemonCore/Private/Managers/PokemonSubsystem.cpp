// "Unreal Pokémon" created by Retro & Chill.

#include "Managers/PokemonSubsystem.h"
#include "Player/PlayerMetadata.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "PokemonCoreSettings.h"
#include "Trainers/TrainerStub.h"

UPokemonSubsystem *UPokemonSubsystem::Instance = nullptr;

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
    Super::Initialize(Collection);

    Instance = this;
    auto Settings = GetDefault<UPokemonCoreSettings>();
    HPStat = Settings->GetHPStat();
    MaxPartySize = Settings->GetMaxPartySize();

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

UPlayerMetadata *UPokemonSubsystem::GetPlayerMetadata() const {
    return PlayerMetadata;
}

const Exp::IGrowthRate &UPokemonSubsystem::GetGrowthRate(FName GrowthRate) const {
    check(GrowthRates.Contains(GrowthRate))
    return *GrowthRates[GrowthRate];
}
