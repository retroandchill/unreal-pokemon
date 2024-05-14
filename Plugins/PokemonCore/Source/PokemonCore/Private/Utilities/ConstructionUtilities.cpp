// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/ConstructionUtilities.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Settings/DependencyInjectionSettings.h"

TScriptInterface<IPokemon> UConstructionUtilities::CreateNewPokemon(const FPokemonDTO &Data) {
    auto Settings = GetDefault<UDependencyInjectionSettings>();
    auto PokemonClass = Settings->GetPokemonClass();
    TScriptInterface<IPokemon> Pokemon = NewObject<UObject>(GetTransientPackage(), PokemonClass);
    Pokemon->Initialize(Data);
    return Pokemon;
}

TScriptInterface<IPokemon> UConstructionUtilities::CreateForeignPokemon(const FPokemonDTO &Data,
                                                                        const TScriptInterface<ITrainer> &Trainer) {
    auto Settings = GetDefault<UDependencyInjectionSettings>();
    auto PokemonClass = Settings->GetPokemonClass();
    TScriptInterface<IPokemon> Pokemon = NewObject<UObject>(GetTransientPackage(), PokemonClass);
    Pokemon->Initialize(Data, Trainer);
    return Pokemon;
}

TScriptInterface<IStatBlock> UConstructionUtilities::CreateStatBlock(const TScriptInterface<IPokemon> &Owner,
                                                                     const FPokemonDTO &DTO) {
    auto Settings = GetDefault<UDependencyInjectionSettings>();
    auto Class = Settings->GetStatBlockClass();
    TScriptInterface<IStatBlock> Ret = NewObject<UObject>(Owner.GetObject(), Class);
    Ret->Initialize(Owner, DTO);
    return Ret;
}

TScriptInterface<IMoveBlock> UConstructionUtilities::CreateMoveBlock(const TScriptInterface<IPokemon> &Owner,
                                                                     const FPokemonDTO &DTO) {
    auto Settings = GetDefault<UDependencyInjectionSettings>();
    auto Class = Settings->GetMoveBlockClass();
    TScriptInterface<IMoveBlock> Ret = NewObject<UObject>(Owner.GetObject(), Class);
    Ret->Initialize(DTO);
    return Ret;
}

TScriptInterface<IAbilityBlock> UConstructionUtilities::CreateAbilityBlock(const TScriptInterface<IPokemon> &Owner,
                                                                           const FPokemonDTO &DTO) {
    auto Settings = GetDefault<UDependencyInjectionSettings>();
    auto Class = Settings->GetAbilityBlockClass();
    TScriptInterface<IAbilityBlock> Ret = NewObject<UObject>(Owner.GetObject(), Class);
    Ret->Initialize(Owner.GetObject(), DTO);
    return Ret;
}
