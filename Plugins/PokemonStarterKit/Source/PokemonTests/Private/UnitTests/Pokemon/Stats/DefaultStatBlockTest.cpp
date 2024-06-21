// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Species/SpeciesData.h"
#include "Utilities/RAII.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultStatBlockTest, "Unit Tests.Core.Stats.DefaultStatBlockTest",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool DefaultStatBlockTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    FPokemonDTO PokemonDTO = {.Species = FName("GARCHOMP"), .Level = 78, .Nature = FName("ADAMANT")};
    PokemonDTO.IVs = {{"HP", 24},  {"ATTACK", 12}, {"DEFENSE", 30}, {"SPECIAL_ATTACK", 16}, {"SPECIAL_DEFENSE", 23},
                      {"SPEED", 5}};

    PokemonDTO.EVs = {{"HP", 74},   {"ATTACK", 190}, {"DEFENSE", 91}, {"SPECIAL_ATTACK", 48}, {"SPECIAL_DEFENSE", 84},
                      {"SPEED", 23}};

    auto NewPokemon = NewObject<UGamePokemon>(World.Get());
    NewPokemon->Initialize(PokemonDTO, nullptr);

    auto Block = NewObject<UDefaultStatBlock>(World.Get());
    Block->Initialize(NewPokemon, PokemonDTO);
    auto &Species = NewPokemon->GetSpecies();
    Block->CalculateStats(Species.BaseStats);
    UE_CHECK_EQUAL(289, Block->GetStat("HP")->GetStatValue());
    UE_CHECK_EQUAL(278, Block->GetStat("ATTACK")->GetStatValue());
    UE_CHECK_EQUAL(193, Block->GetStat("DEFENSE")->GetStatValue());
    UE_CHECK_EQUAL(135, Block->GetStat("SPECIAL_ATTACK")->GetStatValue());
    UE_CHECK_EQUAL(171, Block->GetStat("SPECIAL_DEFENSE")->GetStatValue());
    UE_CHECK_EQUAL(171, Block->GetStat("SPEED")->GetStatValue());
    UE_CHECK_EQUAL(593190, Block->GetExp());
    UE_CHECK_EQUAL(616298, Block->GetExpForNextLevel());

    return true;
}