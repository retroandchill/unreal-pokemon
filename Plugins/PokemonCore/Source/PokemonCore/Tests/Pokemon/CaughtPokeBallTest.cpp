#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/ConstructionUtilities.h"
#include "Utilities/RAII.h"
#include "Pokemon/PokemonDTO.h"
#include "Settings/PokemonSettings.h"

class UPokemonSettings;
IMPLEMENT_SIMPLE_AUTOMATION_TEST(CaughtPokeBallTest, "Unit Tests.Pokemon.CaughtPokeBallTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool CaughtPokeBallTest::RunTest(const FString &Parameters) {
    FGameInstancePtr GameInstance;
    if (!UPokemonSubsystem::Exists()) {
        GameInstance.Reset(NewObject<UGameInstance>());
        GameInstance->Init();
    }

    auto Pokemon1 = UConstructionUtilities::CreateNewPokemon({.Species = "PORYGON"});
    CHECK_EQUAL(GetDefault<UPokemonSettings>()->GetDefaultPokeBall(), Pokemon1->GetPokeBall());

    auto Pokemon2 = UConstructionUtilities::CreateNewPokemon({.Species = "MIMIKYU", .PokeBall = FName("MOONBALL")});
    CHECK_EQUAL(TEXT("MOONBALL"), Pokemon2->GetPokeBall().ToString());
    
    return true;
}
#endif