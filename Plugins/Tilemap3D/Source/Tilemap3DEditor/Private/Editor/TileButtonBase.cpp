// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/TileButtonBase.h"

void UTileButtonBase::NativeOnMouseEnter(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) {
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
    if (auto PressedButtons = InMouseEvent.GetPressedButtons(); PressedButtons.Num() == 1 && PressedButtons.Contains(EKeys::LeftMouseButton)) {
        OnClicked().Broadcast();
    }
}