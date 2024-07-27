// "Unreal Pokémon" created by Retro & Chill.


#include "Components/NamedSlotSelectableWidget.h"
#include "CommonButtonBase.h"
#include "Components/NamedSlot.h"

void UNamedSlotSelectableWidget::NativePreConstruct() {
    Super::NativePreConstruct();

    if (ContentSlot == nullptr) {
        return;
    }
    
    auto ContentWidget = Cast<UPanelWidget>(ContentSlot->GetContent());
    if (ContentWidget == nullptr) {
        return;
    }

    ClearSelectableOptions(false);
    for (int32 i = 0; i < ContentWidget->GetChildrenCount(); i++) {
        auto Child = Cast<UCommonButtonBase>(ContentWidget->GetChildAt(i));
        if (!Child->IsVisible()) {
            continue;
        }

        AddOptionToWidget(Child);
    }
}