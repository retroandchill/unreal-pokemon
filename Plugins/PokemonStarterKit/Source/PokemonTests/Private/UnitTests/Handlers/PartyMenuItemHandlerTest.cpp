#include "Handlers/PartyMenu/PartyMenuItemHandler.h"
#include "Asserts.h"
#include "External/accessor.hpp"
#include "Handlers/PartyMenu/PartyMenuHandlerSet.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Pokemon.h"
#include "Screens/PartyScreen.h"

using namespace fakeit;
using namespace accessor;

MEMBER_ACCESSOR(AccessHandlerHelpText, UPartyMenuItemHandler, HelpText, FText)
MEMBER_ACCESSOR(AccessHandlerSubCommands, UPartyMenuItemHandler, SubCommands, TObjectPtr<UPartyMenuHandlerSet>)
MEMBER_ACCESSOR(AccessHandlerSetHandlers, UPartyMenuHandlerSet, Handlers, TArray<TObjectPtr<UPartyMenuHandler>>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyMenuItemHandlerTest, "Unit Tests.Handlers.PartyMenuItemHandlerTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartyMenuItemHandlerTest::RunTest(const FString &Parameters) {
    // Make the test pass by returning true, or fail by returning false.
    auto [Screen, MockScreen] = UnrealMock::CreateMock<IPartyScreen>();
    auto [Trainer, MockTrainer] = UnrealMock::CreateMock<ITrainer>();
    auto [Pokemon, MockPokemon] = UnrealMock::CreateMock<IPokemon>();

    FItem FakeItem;
    ON_CALL(MockTrainer, GetPokemon).WillByDefault(Return(Pokemon));
    ON_CALL(MockPokemon, GetHoldItem).Return(nullptr).WillByDefault(Return(&FakeItem));

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
    ON_CALL(MockScreen, SetCommandHelpText).Do([&HelpTextOut](FText Text) { HelpTextOut = MoveTemp(Text); });
    ON_CALL(MockScreen, ShowCommands).Do([&HandlersOut](const TArray<TObjectPtr<UPartyMenuHandler>> &Handlers) {
        HandlersOut = Handlers;
    });

    Handler->Handle(Screen, Trainer, 0);
    UE_CHECK_EQUAL(TEXT("Sample help text"), HelpTextOut.ToString());
    UE_ASSERT_EQUAL(1, HandlersOut.Num());
    UE_CHECK_TRUE(HandlersOut[0] == Handler);

    return true;
}