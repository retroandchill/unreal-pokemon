// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Transitions/WildBattleOpponentInfo.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Lookup/InjectionUtilities.h"
#include "Pokemon/Pokemon.h"

FWildBattleOpponentInfo::FWildBattleOpponentInfo(const TSharedRef<FPokemonDTO> &PokemonInfo)
    : OpposingPokemonInfo({PokemonInfo}) {
}

TScriptInterface<IBattleSide> FWildBattleOpponentInfo::CreateOpposingSide(const TScriptInterface<IBattle> &Battle,
                                                                          const TSubclassOf<AActor> &SideClass,
                                                                          const FTransform &Transform,
                                                                          int32 ActivePokemonCount) {
    auto World = Battle.GetObject()->GetWorld();
    check(World != nullptr)

    auto SideActor = World->SpawnActor(SideClass, &Transform);
    SideActor->AttachToActor(CastChecked<AActor>(Battle.GetObject()),
                             FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    TScriptInterface<IBattleSide> Side = SideActor;
    check(Side.GetInterface() != nullptr)

    auto CreatePokemon = [World](const TSharedRef<FPokemonDTO> &PokemonInfo) {
        return UnrealInjector::NewInjectedDependency<IPokemon>(World, *PokemonInfo);
    };

    // clang-format off
    auto Pokemon = OpposingPokemonInfo |
                   Retro::Ranges::Views::Transform(CreatePokemon) |
                   Retro::Ranges::To<TArray>();
    // clang-format on

    // For a wild battle the number of Pokémon and the active side count must match
    check(Pokemon.Num() == ActivePokemonCount)
    return Side->Initialize(Battle, Pokemon);
}