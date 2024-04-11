// "Unreal Pokémon" created by Retro & Chill.
#include "CommonDefines.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/ConstructionUtilities.h"
#include "Utilities/PokemonTestUtilities.h"

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
    AUTOMATION_ASSERT(TestEqual("", Genderless, Pokemon1->GetGender()));

    auto Pokemon2 = UConstructionUtilities::CreateNewPokemon({.Species = "LUCARIO", .PersonalityValue = 0x39593A01});
    AUTOMATION_ASSERT(TestEqual("", Female, Pokemon2->GetGender()));

    auto Pokemon3 = UConstructionUtilities::CreateNewPokemon({.Species = "OSHAWOTT", .PersonalityValue = 0x39593AA3});
    AUTOMATION_ASSERT(TestEqual("", Male, Pokemon3->GetGender()));

    return true;
}
