// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/MessageBoxTestingUtils.h"
#include "Components/CommandWindow.h"
#include "Components/MessageWindow.h"
#include "RPGMenusTestUtilities.h"
#include "RPGUIManagerSubsystem.h"
#include "Screens/TextDisplayScreen.h"
#include "TestAdapter.h"

UE5Coro::TCoroutine<> Pokemon::Testing::AdvanceMessages(const UObject *WorldContextObject, FForceLatentCoroutine Coro) {
    auto Screen = Cast<UTextDisplayScreen>(URPGUIManagerSubsystem::Get(WorldContextObject).GetTopScreenOfOverlay());
    if (Screen == nullptr) {
        co_return;
    }

    while (URPGUIManagerSubsystem::Get(WorldContextObject).GetTopScreenOfOverlay() == Screen) {
        auto MessageWindow = Cast<UMessageWindow>(co_await Screen->AwaitInputPrompt());
        if (MessageWindow == nullptr) {
            co_return;
        }
        URPGMenusTestUtilities::ForceAdvanceText(MessageWindow);
    }
}

UE5Coro::TCoroutine<> Pokemon::Testing::AdvanceMessagesUntilPrompt(const UObject *WorldContextObject,
                                                                   int32 IndexToSelect, FForceLatentCoroutine Coro) {
    auto Screen = Cast<UTextDisplayScreen>(URPGUIManagerSubsystem::Get(WorldContextObject).GetTopScreenOfOverlay());
    if (Screen == nullptr) {
        co_return;
    }

    while (URPGUIManagerSubsystem::Get(WorldContextObject).GetTopScreenOfOverlay() == Screen) {
        auto Window = co_await Screen->AwaitInputPrompt();
        if (auto MessageWindow = Cast<UMessageWindow>(Window); MessageWindow != nullptr) {
            URPGMenusTestUtilities::ForceAdvanceText(MessageWindow);
        } else if (auto CommandWindow = Cast<UCommandWindow>(Window); CommandWindow != nullptr) {
            CommandWindow->SetIndex(IndexToSelect);
            URPGMenusTestUtilities::SelectCurrentOption(CommandWindow);
            co_return;
        } else {
            ADD_ERROR(TEXT("Failed to find a message_window"))
            co_return;
        }
    }
}