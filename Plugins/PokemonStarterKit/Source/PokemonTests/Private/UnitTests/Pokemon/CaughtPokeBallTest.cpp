#include "Asserts.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Settings/BaseSettings.h"
#include "Utilities/RAII.h"
#include "Utilities/WidgetTestUtilities.h"

class UPokemonSettings;
IMPLEMENT_SIMPLE_AUTOMATION_TEST(CaughtPokeBallTest, "Unit Tests.Core.Pokemon.CaughtPokeBallTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool CaughtPokeBallTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "PORYGON"});
    auto &Settings = Pokemon::FBaseSettings::Get();
    UE_CHECK_EQUAL(Settings.GetDefaultPokeBall(), Pokemon1->GetPokeBall());

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "MIMIKYU", .PokeBall = FName("MOONBALL")});
    UE_CHECK_EQUAL(TEXT("MOONBALL"), Pokemon2->GetPokeBall().ToString());

    return true;
}