// "Unreal Pokémon" created by Retro & Chill.
#include "Asserts.h"
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

#define UE_ASSERT_CONTAINS(Type, Element)                                                                              \
    auto Type##Names = U##Type##Helper::Get##Type##Names();                                                            \
    UE_CHECK_TRUE(Type##Names.Contains(Element))

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EditorHelperTest, "Unit Tests.PokemonData.EditorHelperTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool EditorHelperTest::RunTest(const FString &Parameters)
{
    // Make the test pass by returning true, or fail by returning false.

    UE_ASSERT_CONTAINS(Item, TEXT("POTION"));
    UE_ASSERT_CONTAINS(Type, TEXT("NORMAL"));
    UE_ASSERT_CONTAINS(GrowthRate, TEXT("Slow"));
    UE_ASSERT_CONTAINS(Move, TEXT("TACKLE"));
    UE_ASSERT_CONTAINS(Target, TEXT("User"));
    UE_ASSERT_CONTAINS(Ability, TEXT("LEVITATE"));
    UE_ASSERT_CONTAINS(BodyColor, TEXT("Green"));
    UE_ASSERT_CONTAINS(BodyShape, TEXT("Serpentine"));
    UE_ASSERT_CONTAINS(GenderRatio, TEXT("AlwaysMale"));
    UE_ASSERT_CONTAINS(EggGroup, TEXT("Monster"));
    UE_ASSERT_CONTAINS(Evolution, TEXT("LevelMale"));
    UE_ASSERT_CONTAINS(Habitat, TEXT("Grassland"));
    UE_ASSERT_CONTAINS(Nature, TEXT("Quirky"));
    UE_ASSERT_CONTAINS(Species, TEXT("RIOLU"));
    UE_ASSERT_CONTAINS(Stat, TEXT("ACCURACY"));

    auto MainStatNames = UStatHelper::GetMainStatNames();
    UE_CHECK_TRUE(MainStatNames.Contains(TEXT("HP")));
    UE_CHECK_FALSE(MainStatNames.Contains(TEXT("ACCURACY")));

    return true;
}