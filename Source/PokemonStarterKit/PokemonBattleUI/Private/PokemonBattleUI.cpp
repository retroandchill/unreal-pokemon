#include "PokemonBattleUI.h"

DEFINE_LOG_CATEGORY(LogBattleUI)

void FPokemonBattleUIModule::StartupModule()
{
    // No special startup required
}

void FPokemonBattleUIModule::ShutdownModule()
{
    // No special shutdown required
}

IMPLEMENT_MODULE(FPokemonBattleUIModule, PokemonBattleUI)