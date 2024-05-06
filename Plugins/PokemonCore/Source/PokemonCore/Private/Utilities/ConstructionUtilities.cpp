// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/ConstructionUtilities.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "PokemonCoreSettings.h"

TScriptInterface<IPokemon> UConstructionUtilities::CreateNewPokemon(const FPokemonDTO &Data) {
    auto Settings = GetDefault<UPokemonCoreSettings>();
    auto PokemonClass = Settings->GetPokemonClass();
    TScriptInterface<IPokemon> Pokemon = NewObject<UObject>(GetTransientPackage(), PokemonClass);
    Pokemon->Initialize(Data);
    return Pokemon;
}

TScriptInterface<IStatBlock> UConstructionUtilities::CreateStatBlock(const TScriptInterface<IPokemon> &Owner,
                                                                     const FPokemonDTO &DTO) {
    auto Settings = GetDefault<UPokemonCoreSettings>();
    auto Class = Settings->GetStatBlockClass();
    TScriptInterface<IStatBlock> Ret = NewObject<UObject>(Owner.GetObject(), Class);
    Ret->Initialize(Owner, DTO);
    return Ret;
}

TScriptInterface<IMoveBlock> UConstructionUtilities::CreateMoveBlock(const TScriptInterface<IPokemon> &Owner,
                                                                     const FPokemonDTO &DTO) {
    auto Settings = GetDefault<UPokemonCoreSettings>();
    auto Class = Settings->GetMoveBlockClass();
    TScriptInterface<IMoveBlock> Ret = NewObject<UObject>(Owner.GetObject(), Class);
    Ret->Initialize(DTO);
    return Ret;
}

TScriptInterface<IAbilityBlock> UConstructionUtilities::CreateAbilityBlock(const TScriptInterface<IPokemon> &Owner,
    const FPokemonDTO &DTO) {
    auto Settings = GetDefault<UPokemonCoreSettings>();
    auto Class = Settings->GetAbilityBlockClass();
    TScriptInterface<IAbilityBlock> Ret = NewObject<UObject>(Owner.GetObject(), Class);
    Ret->Initialize(Owner.GetObject(), DTO);
    return Ret;
}
