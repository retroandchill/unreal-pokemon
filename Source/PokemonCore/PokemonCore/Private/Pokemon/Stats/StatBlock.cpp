// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/StatBlock.h"

#include "PokemonCoreSettings.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

IMPLEMENT_ABSTRACT_METATYPE(IStatBlock)

FStatBlockRegistry FStatBlockRegistry::Instance;

POKEMONCORE_API TUniquePtr<IStatBlock> CreateStatBlock(FName GrowthRate, uint32 PersonalityValue, const FStatBlockDTO &DTO) {
	auto Settings = GetDefault<UPokemonCoreSettings>();
	auto ClassName = Settings->GetStatBlockClass();
	if (auto &Registry = FStatBlockRegistry::GetInstance(); Registry.IsTypeRegistered(ClassName)) {
		return Registry.Construct(ClassName, GrowthRate, PersonalityValue, DTO);
	}

	// Fallback type
	return MakeUnique<FDefaultStatBlock>(GrowthRate, PersonalityValue, DTO);
}