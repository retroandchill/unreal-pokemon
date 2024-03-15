// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PersonalityValueUtils.h"

#include "Pokemon/PokemonDTO.h"

uint32 UPersonalityValueUtils::GeneratePersonalityValue() {
	auto Lower = static_cast<uint32>(FMath::RandRange(0, LOWER_16_BITS));
	auto Upper = static_cast<uint32>(FMath::RandRange(0, LOWER_16_BITS)) << 16;
	return Lower | Upper;
}

uint32 UPersonalityValueUtils::GeneratePersonalityValue(const FPokemonDTO& DTO) {
	return DTO.bOverride_PersonalityValue ? DTO.PersonalityValue : GeneratePersonalityValue();
}
