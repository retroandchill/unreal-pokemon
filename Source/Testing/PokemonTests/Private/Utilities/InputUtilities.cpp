// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/InputUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Utilities/accessor.hpp"

using namespace accessor;

FUNCTION_ACCESSOR(UserWidgetOnKeyDown, UUserWidget, NativeOnKeyDown, FReply, const FGeometry &, const FKeyEvent &);

FEventReply UInputUtilities::SimulateKeyPress(UUserWidget *Widget, const FKey &Key) {
    FKeyEvent Event(Key, FModifierKeysState(), 0, false, 0, 0);
    auto Reply = callFunction<UserWidgetOnKeyDown>(*Widget, Widget->GetCachedGeometry(), Event);
    return FEventReply(Reply.IsEventHandled());
}
