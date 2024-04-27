// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Species/SpeciesData.h"
#include "Utilities/RAII.h"
#include "Asserts.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultStatBlockTest, "Unit Tests.Core.Stats.DefaultStatBlockTest",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool DefaultStatBlockTest::RunTest(const FString &Parameters) {
    FGameInstancePtr GameInstance;
    if (!UPokemonSubsystem::Exists()) {
        GameInstance.Reset(NewObject<UGameInstance>());
        GameInstance->Init();
    }

    FPokemonDTO PokemonDTO = {.Species = FName("GARCHOMP"), .Level = 78, .Nature = FName("ADAMANT")};
    PokemonDTO.IVs = {{"HP", 24},  {"ATTACK", 12}, {"DEFENSE", 30}, {"SPECIAL_ATTACK", 16}, {"SPECIAL_DEFENSE", 23},
                      {"SPEED", 5}};

    PokemonDTO.EVs = {{"HP", 74},   {"ATTACK", 190}, {"DEFENSE", 91}, {"SPECIAL_ATTACK", 48}, {"SPECIAL_DEFENSE", 84},
                      {"SPEED", 23}};

    auto NewPokemon = NewObject<UGamePokemon>();
    NewPokemon->Initialize(PokemonDTO);

    auto Block = NewObject<UDefaultStatBlock>();
    Block->Initialize(NewPokemon, PokemonDTO);
    auto &Species = NewPokemon->GetSpecies();
    Block->CalculateStats(Species.BaseStats);
    CHECK_EQUAL(289, Block->GetStat("HP")->GetStatValue());
    CHECK_EQUAL(278, Block->GetStat("ATTACK")->GetStatValue());
    CHECK_EQUAL(193, Block->GetStat("DEFENSE")->GetStatValue());
    CHECK_EQUAL(135, Block->GetStat("SPECIAL_ATTACK")->GetStatValue());
    CHECK_EQUAL(171, Block->GetStat("SPECIAL_DEFENSE")->GetStatValue());
    CHECK_EQUAL(171, Block->GetStat("SPEED")->GetStatValue());
    CHECK_EQUAL(593190, Block->GetExp());
    CHECK_EQUAL(616298, Block->GetExpForNextLevel());

    return true;
}
#endif