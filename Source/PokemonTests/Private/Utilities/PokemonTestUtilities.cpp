// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PokemonTestUtilities.h"

#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Moves/DefaultMove.h"
#include "Pokemon/GamePokemon.h"

void UPokemonTestUtilities::CreateMockParty(UObject* WorldContext) {
	using enum EPokemonGender;

	auto PokemonSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UPokemonSubsystem>();
	check(PokemonSubsystem != nullptr);

	auto& Party = PokemonSubsystem->GetPlayer().GetParty();
	Party.Empty();

	TArray<TSharedRef<IMove>> Moves1 = {
										   MakeShared<FDefaultMove>("HYDROPUMP"),
										   MakeShared<FDefaultMove>("KNOCKOFF"),
										   MakeShared<FDefaultMove>("MEGAHORN"),
										   MakeShared<FDefaultMove>("SACREDSWORD")
									   };
	Party.Add(MakeShared<FGamePokemon>(TEXT("SAMUROTT"), 100, Male,
									   TMap<FName, int32>({
										   {"HP", 31},
										   {"ATTACK", 31},
										   {"DEFENSE", 31},
										   {"SPECIAL_ATTACK", 31},
										   {"SPECIAL_DEFENSE", 31},
										   {"SPEED", 31}
									   }),
									   TMap<FName, int32>({
										   {"ATTACK", 164},
										   {"SPECIAL_ATTACK", 92},
										   {"SPEED", 252},
									   }),
									   "Naive", 0, MoveTemp(Moves1), false, "LIFEORB"));

	TArray<TSharedRef<IMove>> Moves2 = {
		MakeShared<FDefaultMove>("FLAREBLITZ"),
		MakeShared<FDefaultMove>("SUPERPOWER"),
		MakeShared<FDefaultMove>("WILDCHARGE"),
		MakeShared<FDefaultMove>("EARTHQUAKE")
	};
	Party.Add(MakeShared<FGamePokemon>(TEXT("EMBOAR"), 100, Female,
									   TMap<FName, int32>({
										   {"HP", 31},
										   {"ATTACK", 31},
										   {"DEFENSE", 31},
										   {"SPECIAL_ATTACK", 31},
										   {"SPECIAL_DEFENSE", 31},
										   {"SPEED", 31}
									   }),
									   TMap<FName, int32>({
										   {"ATTACK", 252},
										   {"DEFENSE", 4},
										   {"SPEED", 252},
									   }),
									   "Jolly", 2, MoveTemp(Moves2), true, "CHOICEBAND"));

	TArray<TSharedRef<IMove>> Moves3 = {
		MakeShared<FDefaultMove>("LEAFSTORM"),
		MakeShared<FDefaultMove>("TAUNT"),
		MakeShared<FDefaultMove>("GLARE"),
		MakeShared<FDefaultMove>("DEFOG")
	};
	Party.Add(MakeShared<FGamePokemon>(TEXT("SERPERIOR"), 100, Male,
									   TMap<FName, int32>({
										   {"HP", 31},
										   {"ATTACK", 0},
										   {"DEFENSE", 31},
										   {"SPECIAL_ATTACK", 31},
										   {"SPECIAL_DEFENSE", 31},
										   {"SPEED", 31}
									   }),
									   TMap<FName, int32>({
										   {"HP", 208},
										   {"DEFENSE", 48},
										   {"SPEED", 252},
									   }),
									   "Timid", 2, MoveTemp(Moves3), false, "ROCKYHELMET"));
}
