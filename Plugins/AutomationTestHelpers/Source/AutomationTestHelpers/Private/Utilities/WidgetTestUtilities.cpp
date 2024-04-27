// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/WidgetTestUtilities.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

UWidget *UWidgetTestUtilities::FindChildWidget(UUserWidget *Parent, FName WidgetName) {
    auto Panel = Cast<UPanelWidget>(Parent->GetRootWidget());
    if (Panel == nullptr) {
        return nullptr;
    }

    int32 Index;
    return UWidgetTree::FindWidgetChild(Panel, WidgetName, Index);
}
