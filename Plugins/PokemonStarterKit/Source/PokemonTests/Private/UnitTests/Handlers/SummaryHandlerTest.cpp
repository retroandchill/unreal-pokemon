
#include "Handlers/PartyMenu/SummaryHandler.h"
#include "Asserts.h"
#include "External/accessor.hpp"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/GamePokemon.h"
#include "RPGMenusSubsystem.h"
#include "Mocks/MockPartyScreen.h"
#include "Screens/PartyScreen.h"
#include "Screens/PokemonSummaryScreen.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/GCPointer.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace accessor;
using namespace testing;

MEMBER_ACCESSOR(AccessSummaryScreen, USummaryHandler, SummaryScreenClass, TSubclassOf<UPokemonSummaryScreen>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SummaryHandlerTest, "Unit Tests.SummaryHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SummaryHandlerTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSummaryScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    CREATE_MOCK(IPartyScreen, Screen, FMockPartyScreen, Mock);
    ON_CALL(Mock, GetPlayerController).WillByDefault(Return(Player->GetPlayerController(nullptr)));

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UGamePokemon::Create(World.Get(), {.Species = TEXT("RIOLU"), .Level = 5}));

    TGCPointer Handler(NewObject<USummaryHandler>());
    UE_CHECK_TRUE(Handler->ShouldShow(Screen, Trainer, 0));
    accessor::accessMember<AccessSummaryScreen>(*Handler).get() = WidgetClass;
    Handler->Handle(Screen, Trainer, 0);

    UE_ASSERT_NOT_NULL(Player->GetSubsystem<URPGMenusSubsystem>()->GetTopOfStack<UPokemonSummaryScreen>());
    return true;
}