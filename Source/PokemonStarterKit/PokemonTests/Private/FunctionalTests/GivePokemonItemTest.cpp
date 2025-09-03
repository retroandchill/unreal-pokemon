// "Unreal Pokémon" created by Retro & Chill.

#include "FunctionalTests/GivePokemonItemTest.h"
#include "Components/Bag/PocketTabWidget.h"
#include "Components/CommandWindow.h"
#include "Components/MessageWindow.h"
#include "Kismet/GameplayStatics.h"
#include "Nodes/Items/GiveItemToPokemon.h"
#include "Player/Bag.h"
#include "Pokemon/Pokemon.h"
#include "RetroLib/Utils/ScopedTimeDilationFactor.h"
#include "RPGMenusTestUtilities.h"
#include "RPGUIManagerSubsystem.h"
#include "Screens/BagScreen.h"
#include "Screens/TextDisplayScreen.h"
#include "TestAdapter.h"
#include "Utilities/MessageBoxTestingUtils.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/RPGMenuUtilities.h"
#include "Utilities/ScopedParty.h"
#include "Utilities/ScopedScreen.h"
#include "Utilities/WidgetTestUtilities.h"

AGivePokemonItemTest::AGivePokemonItemTest() {
    LogWarningHandling = EFunctionalTestLogHandling::OutputIgnored;
    LogErrorHandling = EFunctionalTestLogHandling::OutputIgnored;
}

UE5Coro::TCoroutine<> AGivePokemonItemTest::RunTest(FForceLatentCoroutine) {
    Retro::FScopedTimeDilationFactor TimeDilation(this, 8);
    FPokemonDTO DemoPokemon = {.Species = "Pikachu", .Level = 25};
    Pokemon::Tests::FScopedParty Party(this, Retro::Ranges::Views::Single(std::ref(DemoPokemon)));
    auto Bag = UTrainerHelpers::GetBag(this);
    Bag->ObtainItem("LIGHTBALL", 1);
    int32 LightBallCount = Bag->GetItemQuantity("LIGHTBALL");
    Bag->ObtainItem("LEFTOVERS", 1);
    int32 LeftoversCount = Bag->GetItemQuantity("LEFTOVERS");
    auto Pokemon = Party.GetPokemon(0);
    auto Node = UGiveItemToPokemon::GiveItemToPokemon(this, "LIGHTBALL", Pokemon, 0);

    Node->Activate();
    co_await Race(Pokemon::Testing::AdvanceMessages(this), Node->UntilComplete());

    auto HeldItem = Pokemon->GetHoldItem();
    CO_REQUIRE(HeldItem.IsSet())
    CHECK(HeldItem->ID == "LIGHTBALL")
    CHECK(Bag->GetItemQuantity("LIGHTBALL") == LightBallCount - 1)
    CHECK(Bag->GetItemQuantity("LEFTOVERS") == LeftoversCount)

    Node = UGiveItemToPokemon::GiveItemToPokemon(this, "LEFTOVERS", Pokemon, 0);
    Node->Activate();
    co_await Pokemon::Testing::AdvanceMessagesUntilPrompt(this, 1);
    co_await Race(Pokemon::Testing::AdvanceMessages(this), Node->UntilComplete());

    HeldItem = Pokemon->GetHoldItem();
    CO_REQUIRE(HeldItem.IsSet())
    CHECK(HeldItem->ID == "LIGHTBALL")
    CHECK(Bag->GetItemQuantity("LIGHTBALL") == LightBallCount - 1)
    CHECK(Bag->GetItemQuantity("LEFTOVERS") == LeftoversCount)

    Node = UGiveItemToPokemon::GiveItemToPokemon(this, "LEFTOVERS", Pokemon, 0);
    Node->Activate();
    co_await Pokemon::Testing::AdvanceMessagesUntilPrompt(this, 0);
    co_await Race(Pokemon::Testing::AdvanceMessages(this), Node->UntilComplete());

    HeldItem = Pokemon->GetHoldItem();
    CO_REQUIRE(HeldItem.IsSet())
    CHECK(HeldItem->ID == "LEFTOVERS")
    CHECK(Bag->GetItemQuantity("LIGHTBALL") == LightBallCount)
    CHECK(Bag->GetItemQuantity("LEFTOVERS") == LeftoversCount - 1)
}