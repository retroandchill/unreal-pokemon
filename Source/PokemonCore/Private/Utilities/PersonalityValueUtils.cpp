// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PersonalityValueUtils.h"

uint32 UPersonalityValueUtils::GeneratePersonalityValue() {
	auto Lower = static_cast<uint32>(FMath::RandRange(0, LOWER_16_BITS));
	auto Upper = static_cast<uint32>(FMath::RandRange(0, LOWER_16_BITS)) << 16;
	return Lower | Upper;
}
