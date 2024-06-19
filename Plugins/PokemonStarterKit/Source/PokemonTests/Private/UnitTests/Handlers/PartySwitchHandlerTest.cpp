// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Handlers/PartyMenu/PartyMenuHandler.h"
#include "Handlers/PartyMenu/PartySwitchHandler.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/GamePokemon.h"
#include "Screens/PartyScreen.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/GCPointer.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartySwitchHandlerTest, "Unit Tests.UI.PartySwitchHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartySwitchHandlerTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto [Screen, Mock] = UnrealMock::CreateMock<IPartyScreen>(World.Get());
    TOptional<int32> SwitchIndex;
    ON_CALL(Mock, BeginSwitch).Do([&SwitchIndex](int32 Index) { SwitchIndex.Emplace(Index); });

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UGamePokemon::Create(World.Get(), {.Species = TEXT("RIOLU"), .Level = 5}));

    TGCPointer<UPartyMenuHandler> Handler(NewObject<UPartySwitchHandler>());
    UE_CHECK_FALSE(Handler->ShouldShow(Screen, Trainer, 0));

    Trainer->AddPokemonToParty(UGamePokemon::Create(World.Get(), {.Species = TEXT("OSHAWOTT"), .Level = 5}));
    UE_CHECK_TRUE(Handler->ShouldShow(Screen, Trainer, 0));

    Handler->Handle(Screen, Trainer, 0);
    UE_ASSERT_TRUE(SwitchIndex.IsSet());
    UE_CHECK_EQUAL(0, SwitchIndex.GetValue());

    return true;
}