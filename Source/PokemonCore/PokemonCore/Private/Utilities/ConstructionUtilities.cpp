// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/ConstructionUtilities.h"

#include "PokemonCoreSettings.h"
#include "Pokemon/Pokemon.h"

TScriptInterface<IPokemon> UConstructionUtilities::CreateNewPokemon(const FPokemonDTO& Data) {
	auto Settings = GetDefault<UPokemonCoreSettings>();
	auto PokemonClass = Settings->GetPokemonClass();
	TScriptInterface<IPokemon> Pokemon = NewObject<UObject>(GetTransientPackage(), PokemonClass);
	Pokemon->Initialize(Data);
	return Pokemon;
}
