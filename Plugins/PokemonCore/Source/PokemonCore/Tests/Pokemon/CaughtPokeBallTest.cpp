#include "Lookup/InjectionUtilities.h"
#include "Utilities/WidgetTestUtilities.h"
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Settings/PokemonSettings.h"
#include "Utilities/RAII.h"

class UPokemonSettings;
IMPLEMENT_SIMPLE_AUTOMATION_TEST(CaughtPokeBallTest, "Unit Tests.Core.Pokemon.CaughtPokeBallTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool CaughtPokeBallTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(World, FPokemonDTO{.Species = "PORYGON"});
    CHECK_EQUAL(GetDefault<UPokemonSettings>()->GetDefaultPokeBall(), Pokemon1->GetPokeBall());

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World, FPokemonDTO{.Species = "MIMIKYU", .PokeBall = FName("MOONBALL")});
    CHECK_EQUAL(TEXT("MOONBALL"), Pokemon2->GetPokeBall().ToString());

    return true;
}
#endif