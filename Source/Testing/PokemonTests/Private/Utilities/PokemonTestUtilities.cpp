// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PokemonTestUtilities.h"

#include "Asserts.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "Utilities/ConstructionUtilities.h"
#include "Pokemon/PokemonDTO.h"

void UPokemonTestUtilities::CreateMockParty(UObject* WorldContext) {
	using enum EPokemonGender;

	auto PokemonSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UPokemonSubsystem>();
	ASSERT(PokemonSubsystem != nullptr);

	auto Player = PokemonSubsystem->GetPlayer();
	Player->ClearParty();
	
	Player->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({
		.Species = FName("SAMUROTT"),
		.Level = 100,
		.Gender = Male,
		.Shiny = false,
		.IVs = {
			{"HP", 31},
			{"ATTACK", 31},
			{"DEFENSE", 31},
			{"SPECIAL_ATTACK", 31},
			{"SPECIAL_DEFENSE", 31},
			{"SPEED", 31}
		},
		.EVs = {
			{"ATTACK", 164},
			{"SPECIAL_ATTACK", 92},
			{"SPEED", 252}
		},
		.Nature = FName("Naive"),
		.Ability = 0,
		.Item = FName("LIFEORB"),
		.Moves = {"HYDROPUMP", "KNOCKOFF", "MEGAHORN", "SACREDSWORD"}
	}));
	Player->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({
		.Species = FName("EMBOAR"),
		.Level = 100,
		.Gender = Female,
		.Shiny = true,
		.IVs = {
			{"HP", 31},
			{"ATTACK", 31},
			{"DEFENSE", 31},
			{"SPECIAL_ATTACK", 31},
			{"SPECIAL_DEFENSE", 31},
			{"SPEED", 31}
		},
		.EVs = {
			{"ATTACK", 252},
			{"DEFENSE", 4},
			{"SPEED", 252}
		},
		.Nature = FName("Jolly"),
		.Ability = 2,
		.Item = FName("CHOICEBAND"),
		.Moves = {"FLAREBLITZ", "SUPERPOWER", "WILDCHARGE", "EARTHQUAKE"}
	}));
	Player->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({
		.Species = FName("SERPERIOR"),
		.Level = 100,
		.Gender = Female,
		.Shiny = false,
		.IVs = {
			{"HP", 31},
			{"ATTACK", 0},
			{"DEFENSE", 31},
			{"SPECIAL_ATTACK", 31},
			{"SPECIAL_DEFENSE", 31},
			{"SPEED", 31}
		},
		.EVs = {
			{"HP", 208},
			{"DEFENSE", 48},
			{"SPEED", 252}
		},
		.Nature = FName("Timid"),
		.Ability = 2,
		.Item = FName("ROCKYHELMET"),
		.Moves = {"LEAFSTORM", "TAUNT", "GLARE", "DEFOG"}
	}));
}
