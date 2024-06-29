#include "PokemonBattleModule.h"
#include "Battle/Types/TypeTags.h"

DEFINE_LOG_CATEGORY(LogBattle)

void FPokemonBattleModule::StartupModule() {
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FPokemonBattleModule::OnPostEngineInit);
}

void FPokemonBattleModule::ShutdownModule() {
    // No shutdown code required
}

void FPokemonBattleModule::OnPostEngineInit() {
    // We basically need to force the singleton to construct itself
    Pokemon::Battle::Types::FLookup::GetInstance();
}

IMPLEMENT_MODULE(FPokemonBattleModule, PokemonBattle)