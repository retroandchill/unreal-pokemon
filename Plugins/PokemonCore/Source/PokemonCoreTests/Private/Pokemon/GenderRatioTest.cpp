// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/RAII.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GenderRatioTest, "Unit Tests.Core.Pokemon.GenderRatioTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GenderRatioTest::RunTest(const FString &Parameters) {
    using enum EPokemonGender;
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "PORYGON"});
    ASSERT_EQUAL(Pokemon1->GetGender(), Genderless);

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "LUCARIO", .PersonalityValue = 0x39593A01});
    ASSERT_EQUAL(Pokemon2->GetGender(), Female);

    auto Pokemon3 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "OSHAWOTT", .PersonalityValue = 0x39593AA3});
    ASSERT_EQUAL(Pokemon3->GetGender(), Male);

    return true;
}