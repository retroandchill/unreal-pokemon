// "Unreal Pokémon" created by Retro & Chill.

#include "PokemonTestUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/PartyManagementHelpers.h"
#include "Utilities/TrainerHelpers.h"

void FPokemonTestUtilities::CreateMockParty(UObject *WorldContext) {
    using enum EPokemonGender;

    auto Player = UTrainerHelpers::GetPlayerCharacter(WorldContext);
    Player->ClearParty();

    UPartyManagementHelpers::AddPokemonToParty(WorldContext,
                                               {.Species = FName("SAMUROTT"),
                                                .Level = 100,
                                                .Gender = Male,
                                                .Shiny = false,
                                                .IVs = {{"HP", 31},
                                                        {"ATTACK", 31},
                                                        {"DEFENSE", 31},
                                                        {"SPECIAL_ATTACK", 31},
                                                        {"SPECIAL_DEFENSE", 31},
                                                        {"SPEED", 31}},
                                                .EVs = {{"ATTACK", 164}, {"SPECIAL_ATTACK", 92}, {"SPEED", 252}},
                                                .Nature = FName("Naive"),
                                                .Ability = FName("TORRENT"),
                                                .Item = FName("LIFEORB"),
                                                .Moves = {"HYDROPUMP", "KNOCKOFF", "MEGAHORN", "SACREDSWORD"}});
    UPartyManagementHelpers::AddPokemonToParty(WorldContext,
                                               {.Species = FName("EMBOAR"),
                                                .Level = 100,
                                                .Gender = Female,
                                                .Shiny = true,
                                                .IVs = {{"HP", 31},
                                                        {"ATTACK", 31},
                                                        {"DEFENSE", 31},
                                                        {"SPECIAL_ATTACK", 31},
                                                        {"SPECIAL_DEFENSE", 31},
                                                        {"SPEED", 31}},
                                                .EVs = {{"ATTACK", 252}, {"DEFENSE", 4}, {"SPEED", 252}},
                                                .Nature = FName("Jolly"),
                                                .Ability = FName("RECKLESS"),
                                                .Item = FName("CHOICEBAND"),
                                                .Moves = {"FLAREBLITZ", "SUPERPOWER", "WILDCHARGE", "EARTHQUAKE"}});
    UPartyManagementHelpers::AddPokemonToParty(WorldContext, {.Species = FName("SERPERIOR"),
                                                              .Level = 100,
                                                              .Gender = Female,
                                                              .Shiny = false,
                                                              .IVs = {{"HP", 31},
                                                                      {"ATTACK", 0},
                                                                      {"DEFENSE", 31},
                                                                      {"SPECIAL_ATTACK", 31},
                                                                      {"SPECIAL_DEFENSE", 31},
                                                                      {"SPEED", 31}},
                                                              .EVs = {{"HP", 208}, {"DEFENSE", 48}, {"SPEED", 252}},
                                                              .Nature = FName("Timid"),
                                                              .Ability = FName("CONTRARY"),
                                                              .Item = FName("ROCKYHELMET"),
                                                              .Moves = {"LEAFSTORM", "TAUNT", "GLARE", "DEFOG"}});
}