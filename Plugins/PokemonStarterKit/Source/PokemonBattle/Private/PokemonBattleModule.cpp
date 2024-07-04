#include "PokemonBattleModule.h"
#include "Battle/Moves/MoveTags.h"
#include "Battle/StatusEffects/StatusEffectTags.h"
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
    Pokemon::Battle::Moves::FLookup::GetInstance();
    Pokemon::Battle::Types::FLookup::GetInstance();
    Pokemon::Battle::StatusEffects::FLookup::GetInstance();
}

IMPLEMENT_MODULE(FPokemonBattleModule, PokemonBattle)