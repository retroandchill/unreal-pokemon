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

using namespace testing;
using namespace accessor;

MEMBER_ACCESSOR(AccessHandlerHelpText, UPartyMenuItemHandler, HelpText, FText)
MEMBER_ACCESSOR(AccessHandlerSubCommands, UPartyMenuItemHandler, SubCommands, TObjectPtr<UPartyMenuHandlerSet>)
MEMBER_ACCESSOR(AccessHandlerSetHandlers, UPartyMenuHandlerSet, Handlers, TArray<TObjectPtr<UPartyMenuHandler>>)

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

    accessMember<AccessHandlerHelpText>(*Handler).get() = FText::FromStringView(TEXT("Sample help text"));
    auto &HandlerSet = accessMember<AccessHandlerSubCommands>(*Handler).get();
    HandlerSet = NewObject<UPartyMenuHandlerSet>();
    accessMember<AccessHandlerSetHandlers>(*HandlerSet).get() = {Handler};
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