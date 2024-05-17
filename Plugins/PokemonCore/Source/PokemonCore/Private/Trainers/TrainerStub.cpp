// "Unreal Pokémon" created by Retro & Chill.

#include "Trainers/TrainerStub.h"
#include "Pokemon/GamePokemon.h"
#include "Utilities/ConstructionUtilities.h"

TScriptInterface<ITrainer> UTrainerStub::Initialize(FName NewTrainerType, FText NewTrainerName) {
    Super::Initialize(NewTrainerType, NewTrainerName);

    AddPokemonToParty(UConstructionUtilities::CreateForeignPokemon({.Species = "RIOLU",
                                                                    .Level = 10,
                                                                    .Shiny = true,
                                                                    .Item = FName("BLACKBELT"),
                                                                    .ObtainMethod = EObtainMethod::FatefulEncounter},
                                                                   this));
    AddPokemonToParty(UConstructionUtilities::CreateForeignPokemon({.Species = "SNIVY", .Level = 10}, this));
    AddPokemonToParty(UConstructionUtilities::CreateForeignPokemon({.Species = "TEPIG", .Level = 10}, this));
    AddPokemonToParty(UConstructionUtilities::CreateForeignPokemon({.Species = "OSHAWOTT", .Level = 10}, this));
    AddPokemonToParty(
        UConstructionUtilities::CreateForeignPokemon({.Species = "FRILLISH",
                                                      .Level = 10,
                                                      .Gender = EPokemonGender::Male,
                                                      .MetLocation = FText::FromStringView(TEXT("Test Map"))},
                                                     this));
    AddPokemonToParty(UConstructionUtilities::CreateForeignPokemon(
        {.Species = "FRILLISH", .Level = 10, .Gender = EPokemonGender::Female}, this));

    return this;
}