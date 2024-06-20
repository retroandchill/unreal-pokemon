// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/InputUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Slate/SObjectWidget.h"

FEventReply UInputUtilities::SimulateKeyPress(UUserWidget *Widget, const FKey &Key) {
    FKeyEvent Event(Key, FModifierKeysState(), 0, false, 0, 0);
    auto ObjectWidget = MakeShared<SObjectWidget>();
    ObjectWidget->Construct({}, Widget);
    return Widget->OnKeyDown(FGeometry(), Event);
}
