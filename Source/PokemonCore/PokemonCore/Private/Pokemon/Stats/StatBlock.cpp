// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/StatBlock.h"

#include "PokemonCoreSettings.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

POKEMONCORE_API TScriptInterface<IStatBlock> CreateStatBlock(const TScriptInterface<IPokemon>& Owner, const FPokemonDTO &DTO) {
	auto Settings = GetDefault<UPokemonCoreSettings>();
	auto Class = Settings->GetStatBlockClass();
	TScriptInterface<IStatBlock> Ret = NewObject<UObject>(Owner.GetObject(), Class);
	Ret->Initialize(Owner, DTO);
	return Ret;
}