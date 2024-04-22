// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Assertions/Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/ConstructionUtilities.h"
#include "Utilities/RAII.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GenderRatioTest, "Private.PokemonCore.Pokemon.GenderRatioTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GenderRatioTest::RunTest(const FString &Parameters) {
    using enum EPokemonGender;
    FGameInstancePtr GameInstance;
    if (!UPokemonSubsystem::Exists()) {
        GameInstance.Reset(NewObject<UGameInstance>());
        GameInstance->Init();
    }

    auto Pokemon1 = UConstructionUtilities::CreateNewPokemon({.Species = "PORYGON"});
    ASSERT_EQUAL(Pokemon1->GetGender(), Genderless);

    auto Pokemon2 = UConstructionUtilities::CreateNewPokemon({.Species = "LUCARIO", .PersonalityValue = 0x39593A01});
    ASSERT_EQUAL(Pokemon2->GetGender(), Female);

    auto Pokemon3 = UConstructionUtilities::CreateNewPokemon({.Species = "OSHAWOTT", .PersonalityValue = 0x39593AA3});
    ASSERT_EQUAL(Pokemon3->GetGender(), Male);

    return true;
}
#endif