// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonCore.h"
#include "Player/Sorting/BagSorter.h"
#include "Player/Sorting/IndexedBagSorter.h"
#include "Player/Sorting/NamedBagSorter.h"
#include "Pokemon/Exp/Erratic.h"
#include "Pokemon/Exp/Fast.h"
#include "Pokemon/Exp/Fluctuating.h"
#include "Pokemon/Exp/Medium.h"
#include "Pokemon/Exp/Parabolic.h"
#include "Pokemon/Exp/Slow.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

constexpr auto GLoctextNamespace = "FPokemonCoreModule";

void FPokemonCoreModule::StartupModule()
{
    auto &GrowthRateRegistry = Exp::FGrowthRateRegistry::GetInstance();
    GrowthRateRegistry.RegisterClass<Exp::FMedium>("Medium");
    GrowthRateRegistry.RegisterClass<Exp::FErratic>("Erratic");
    GrowthRateRegistry.RegisterClass<Exp::FFluctuating>("Fluctuating");
    GrowthRateRegistry.RegisterClass<Exp::FParabolic>("Parabolic");
    GrowthRateRegistry.RegisterClass<Exp::FFast>("Fast");
    GrowthRateRegistry.RegisterClass<Exp::FSlow>("Slow");

    auto &BagSorterRegistry = FBagSorterRegistry::GetInstance();
    BagSorterRegistry.RegisterClass<FIndexedBagSorter>("IndexedBagSorter");
    BagSorterRegistry.RegisterClass<FNamedBagSorter>("NamedBagSorter");
}

void FPokemonCoreModule::ShutdownModule()
{
    // No implementation needed
}

IMPLEMENT_MODULE(FPokemonCoreModule, PokemonCore)
