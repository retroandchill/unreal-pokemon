#include "Handlers/PartyMenu/PartyMenuItemHandler.h"
#include "Asserts.h"
#include "External/accessor.hpp"
#include "Handlers/PartyMenu/PartyMenuHandlerSet.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockPartyScreen.h"
#include "Mocks/MockPokemon.h"
#include "Mocks/MockTrainer.h"
#include "Pokemon/Pokemon.h"
#include "Screens/PartyScreen.h"
#include "Utilities/ReflectionUtils.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyMenuItemHandlerTest, "Unit Tests.Handlers.PartyMenuItemHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartyMenuItemHandlerTest::RunTest(const FString &Parameters) {
    // Make the test pass by returning true, or fail by returning false.
    CREATE_MOCK(IPartyScreen, Screen, FMockPartyScreen, MockScreen);
    CREATE_MOCK(ITrainer, Trainer, FMockTrainer, MockTrainer);
    CREATE_MOCK(IPokemon, Pokemon, FMockPokemon, MockPokemon);

    FItem FakeItem;
    ON_CALL(MockTrainer, GetPokemon).WillByDefault(Return(Pokemon));
    EXPECT_CALL(MockPokemon, GetHoldItem).WillOnce(Return(nullptr)).WillRepeatedly(Return(&FakeItem));

    auto Handler = NewObject<UPartyMenuItemHandler>();
    UE_CHECK_TRUE(Handler->ShouldShow(Screen, Trainer, 0));
    UE_CHECK_TRUE(Handler->ShouldShow(Screen, Trainer, 0));
    FakeItem.Tags.Add(TEXT("Mail"));
    UE_CHECK_FALSE(Handler->ShouldShow(Screen, Trainer, 0));

    UReflectionUtils::SetPropertyValue(Handler, "HelpText", FText::FromStringView(TEXT("Sample help text")));
    auto& HandlerSet = UReflectionUtils::GetPropertyValue<TObjectPtr<UPartyMenuHandlerSet>>(Handler, "SubCommands");
    UReflectionUtils::SetPropertyValue<TObjectPtr<UPartyMenuHandlerSet>>(Handler, "SubCommands", NewObject<UPartyMenuHandlerSet>());
    UReflectionUtils::SetPropertyValue<TArray<TObjectPtr<UPartyMenuHandler>>>(HandlerSet, "Handlers", {Handler});
    FText HelpTextOut;
    TArray<TObjectPtr<UPartyMenuHandler>> HandlersOut;
    ON_CALL(MockScreen, SetCommandHelpText).WillByDefault([&HelpTextOut](FText Text) { HelpTextOut = MoveTemp(Text); });
    ON_CALL(MockScreen, ShowCommands).WillByDefault([&HandlersOut](const TArray<TObjectPtr<UPartyMenuHandler>> &Handlers) {
        HandlersOut = Handlers;
    });

    Handler->Handle(Screen, Trainer, 0);
    UE_CHECK_EQUAL(TEXT("Sample help text"), HelpTextOut.ToString());
    UE_ASSERT_EQUAL(1, HandlersOut.Num());
    UE_CHECK_TRUE(HandlersOut[0] == Handler);

    return true;
}