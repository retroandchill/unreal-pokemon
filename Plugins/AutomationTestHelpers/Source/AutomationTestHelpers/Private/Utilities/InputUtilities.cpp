// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/InputUtilities.h"
#include "Blueprint/UserWidget.h"

template <typename T>
class TInputPublicWidget : public T {
public:
    FReply ForceAccessOnKeyDown(const FGeometry& Geometry, const FKeyEvent& KeyEvent) {
        return this->NativeOnKeyDown(Geometry, KeyEvent);
    }
};

FEventReply UInputUtilities::SimulateKeyPress(UUserWidget *Widget, const FKey &Key) {
    FKeyEvent Event(Key, FModifierKeysState(), 0, false, 0, 0);
    auto CastWidget = reinterpret_cast<TInputPublicWidget<UUserWidget>*>(Widget);
    auto Reply = CastWidget->ForceAccessOnKeyDown(FGeometry(), Event);
    return FEventReply(Reply.IsEventHandled());
}
