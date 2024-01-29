#include "PokemonData.h"

#include "DataRegistry.h"
#include "Exp/Erratic.h"
#include "Exp/GrowthRate.h"
#include "Exp/Medium.h"

#define LOCTEXT_NAMESPACE "FPokemonDataModule"

void FPokemonDataModule::StartupModule() {
}

void FPokemonDataModule::ShutdownModule() {
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPokemonDataModule, PokemonData)
