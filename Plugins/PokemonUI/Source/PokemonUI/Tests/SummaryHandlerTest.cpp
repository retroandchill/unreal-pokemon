
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Handlers/PartyMenu/SummaryHandler.h"
#include "Asserts.h"
#include "MockScreen.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/GCPointer.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Screens/PokemonSummaryScreen.h"
#include "External/accessor.hpp"

using namespace accessor;

MEMBER_ACCESSOR(AccessSummaryScreen, USummaryHandler, SummaryScreenClass, TSubclassOf<UPokemonSummaryScreen>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SummaryHandlerTest, "Unit Tests.SummaryHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SummaryHandlerTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSummaryScreen>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];
    
    auto PlayerController = World->SpawnActor<ARPGPlayerController>();
    auto Pawn = World->SpawnActor<APawn>();
    auto Player = NewObject<ULocalPlayer>(GEngine);
    PlayerController->Possess(Pawn);
    PlayerController->Player = Player;
    FMockScreen Screen(PlayerController);

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UGamePokemon::Create({.Species = TEXT("RIOLU"), .Level = 5}));

    TGCPointer Handler(NewObject<USummaryHandler>());
    CHECK_TRUE(Handler->ShouldShow(Screen, *Trainer, 0));
    accessor::accessMember<AccessSummaryScreen>(*Handler).get() = WidgetClass;
    Handler->Handle(Screen, *Trainer, 0);

    ASSERT_NOT_NULL(PlayerController->GetTopOfStack<UPokemonSummaryScreen>());
    return true;
}
#endif