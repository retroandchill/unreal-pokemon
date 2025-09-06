
#include "Asserts.h"
#include "Dispatchers/TestDispatcher.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

constexpr auto TEST_HOLD_ITEM_EXECUTOR =
    TEXT("/PokemonStarterKit/Tests/Resources/GetHoldItemTestHelper.GetHoldItemTestHelper");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetHoldItemTest_Null, "Unit Tests.Core.Pokemon.GetHoldItemTest.InvalidPokemon",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetHoldItemTest_Null::RunTest(const FString &Parameters)
{
    auto TestHelper = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_HOLD_ITEM_EXECUTOR);
    UE_ASSERT_NOT_NULL(TestHelper);
    auto Dispatcher = NewObject<UObject>(GetTransientPackage(), TestHelper);
    AddExpectedError(TEXT("Get Held Item called on an invalid Pokémon reference!"));
    ITestDispatcher::Execute_ExecuteTest(Dispatcher);
    bool HasItem = UReflectionUtils::GetPropertyValue<bool>(Dispatcher, TEXT("HasItem"));
    UE_CHECK_FALSE(HasItem);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetHoldItemTest_WithNoItem, "Unit Tests.Core.Pokemon.GetHoldItemTest.WithNoItem",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetHoldItemTest_WithNoItem::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto TestHelper = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_HOLD_ITEM_EXECUTOR);
    UE_ASSERT_NOT_NULL(TestHelper);

    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "PIKACHU"});
    auto Dispatcher = NewObject<UObject>(GetTransientPackage(), TestHelper);
    UReflectionUtils::SetPropertyValue(Dispatcher, TEXT("Pokemon"), Pokemon);
    ITestDispatcher::Execute_ExecuteTest(Dispatcher);
    bool HasItem = UReflectionUtils::GetPropertyValue<bool>(Dispatcher, TEXT("HasItem"));
    UE_CHECK_FALSE(HasItem);
    UE_CHECK_FALSE(Pokemon->GetHoldItem().IsSet());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetHoldItemTest_WithItem, "Unit Tests.Core.Pokemon.GetHoldItemTest.WithItem",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetHoldItemTest_WithItem::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto TestHelper = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_HOLD_ITEM_EXECUTOR);
    UE_ASSERT_NOT_NULL(TestHelper);

    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "PIKACHU", .Item = FName(TEXT("LIGHTBALL"))});
    auto Dispatcher = NewObject<UObject>(GetTransientPackage(), TestHelper);
    UReflectionUtils::SetPropertyValue(Dispatcher, TEXT("Pokemon"), Pokemon);
    ITestDispatcher::Execute_ExecuteTest(Dispatcher);
    bool HasItem = UReflectionUtils::GetPropertyValue<bool>(Dispatcher, TEXT("HasItem"));
    UE_CHECK_TRUE(HasItem);
    UE_CHECK_TRUE(Pokemon->GetHoldItem().IsSet());

    auto &HoldItem = UReflectionUtils::GetPropertyValue<FItem>(Dispatcher, TEXT("HoldItem"));
    UE_CHECK_EQUAL(TEXT("LIGHTBALL"), HoldItem.ID.ToString());
    UE_CHECK_TRUE(Pokemon->GetHoldItem().IsSet());
    UE_CHECK_EQUAL(TEXT("LIGHTBALL"), Pokemon->GetHoldItem()->ID.ToString());

    return true;
}