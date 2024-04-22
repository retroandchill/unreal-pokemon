// "Unreal Pokémon" created by Retro & Chill.
#include "Assertions/Asserts.h"
#if WITH_TESTS && HAS_AUTOMATION_HELPERS

#include "Bag/Item.h"
#include "Battle/Type.h"
#include "Exp/GrowthRateData.h"
#include "Misc/AutomationTest.h"
#include "Moves/MoveData.h"
#include "Moves/Target.h"
#include "Species/Ability.h"
#include "Species/BodyColor.h"
#include "Species/BodyShape.h"
#include "Species/EggGroup.h"
#include "Species/EvolutionData.h"
#include "Species/GenderRatio.h"
#include "Species/Habitat.h"
#include "Species/Nature.h"
#include "Species/SpeciesData.h"
#include "Species/Stat.h"

#define ASSERT_CONTAINS(Type, Element)                                                                                 \
    auto Type##Names = U##Type##Helper::Get##Type##Names();                                                            \
    CHECK_TRUE(Type##Names.Contains(Element))

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EditorHelperTest, "UnrealPokemon.PokemonData.EditorHelperTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool EditorHelperTest::RunTest(const FString &Parameters) {
    // Make the test pass by returning true, or fail by returning false.

    ASSERT_CONTAINS(Item, TEXT("POTION"));
    ASSERT_CONTAINS(Type, TEXT("NORMAL"));
    ASSERT_CONTAINS(GrowthRate, TEXT("Slow"));
    ASSERT_CONTAINS(Move, TEXT("TACKLE"));
    ASSERT_CONTAINS(Target, TEXT("User"));
    ASSERT_CONTAINS(Ability, TEXT("LEVITATE"));
    ASSERT_CONTAINS(BodyColor, TEXT("Green"));
    ASSERT_CONTAINS(BodyShape, TEXT("Serpentine"));
    ASSERT_CONTAINS(GenderRatio, TEXT("AlwaysMale"));
    ASSERT_CONTAINS(EggGroup, TEXT("Monster"));
    ASSERT_CONTAINS(Evolution, TEXT("LevelMale"));
    ASSERT_CONTAINS(Habitat, TEXT("Grassland"));
    ASSERT_CONTAINS(Nature, TEXT("Quirky"));
    ASSERT_CONTAINS(Species, TEXT("RIOLU"));
    ASSERT_CONTAINS(Stat, TEXT("ACCURACY"));

    auto MainStatNames = UStatHelper::GetMainStatNames();
    CHECK_TRUE(MainStatNames.Contains(TEXT("HP")));
    CHECK_FALSE(MainStatNames.Contains(TEXT("ACCURACY")));

    return true;
}

#endif