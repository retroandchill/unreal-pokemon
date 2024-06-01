#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Handlers/PartyMenu/SummaryHandler.h"
#include "Asserts.h"
#include "External/accessor.hpp"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "PokemonUI/Tests/MockScreen.h"
#include "RPGMenusSubsystem.h"
#include "Screens/PokemonSummaryScreen.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/GCPointer.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessSummaryScreen, USummaryHandler, SummaryScreenClass, TSubclassOf<UPokemonSummaryScreen>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SummaryHandlerTest, "Unit Tests.SummaryHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SummaryHandlerTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSummaryScreen>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto PlayerController = World->SpawnActor<APlayerController>();
    auto Pawn = World->SpawnActor<APawn>();
    auto Player = NewObject<ULocalPlayer>(GEngine);
    PlayerController->Possess(Pawn);
    PlayerController->Player = Player;
    FMockScreen Screen(PlayerController);

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UGamePokemon::Create(World.Get(), {.Species = TEXT("RIOLU"), .Level = 5}));

    TGCPointer Handler(NewObject<USummaryHandler>());
    CHECK_TRUE(Handler->ShouldShow(Screen, *Trainer, 0));
    accessor::accessMember<AccessSummaryScreen>(*Handler).get() = WidgetClass;
    Handler->Handle(Screen, *Trainer, 0);

    ASSERT_NOT_NULL(Player->GetSubsystem<URPGMenusSubsystem>()->GetTopOfStack<UPokemonSummaryScreen>());
    return true;
}
#endif