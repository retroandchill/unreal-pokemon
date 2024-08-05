
#include "Asserts.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Trainers/BasicTrainer.h"
#include "Trainers/OwnerInfo.h"
#include "Trainers/TrainerType.h"
#include "Utilities/RAII.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TrainerInfoTest, "Unit Tests.Core.Pokemon.TrainerInfoTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TrainerInfoTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    UPokemonSubsystem::GetInstance(World.Get()).StartNewGame();
    auto Player = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    UE_ASSERT_NOT_NULL(Player.GetObject());
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "PORYGON"});
    UE_CHECK_EQUAL(Player->GetTrainerName().ToString(), Pokemon1->GetOwnerInfo().OriginalTrainerName.ToString());
    UE_CHECK_EQUAL(Player->GetTrainerType().Gender, Pokemon1->GetOwnerInfo().OriginalTrainerGender);
    UE_CHECK_EQUAL(Player->GetIdNumber(), Pokemon1->GetOwnerInfo().ID);
    UE_CHECK_EQUAL(Player->GetSecretId(), Pokemon1->GetOwnerInfo().SecretID);

    auto ForeignTrainer = NewObject<UBasicTrainer>(World.Get())->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "PORYGON"}, ForeignTrainer);
    UE_CHECK_EQUAL(TEXT("Amy"), Pokemon2->GetOwnerInfo().OriginalTrainerName.ToString());
    UE_CHECK_EQUAL(ETrainerGender::Female, Pokemon2->GetOwnerInfo().OriginalTrainerGender);
    UE_CHECK_EQUAL(ForeignTrainer->GetIdNumber(), Pokemon2->GetOwnerInfo().ID);
    UE_CHECK_EQUAL(ForeignTrainer->GetSecretId(), Pokemon2->GetOwnerInfo().SecretID);

    return true;
}