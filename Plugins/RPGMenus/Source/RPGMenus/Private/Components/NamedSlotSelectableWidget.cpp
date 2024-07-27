// "Unreal Pokémon" created by Retro & Chill.


#include "Components/NamedSlotSelectableWidget.h"
#include "CommonButtonBase.h"
#include "Components/NamedSlot.h"
#include "Components/RPGButtonBase.h"

void UNamedSlotSelectableWidget::NativePreConstruct() {
    Super::NativePreConstruct();
    UpdateOptions();
}

void UNamedSlotSelectableWidget::UpdateOptions() {
#if WITH_EDITOR
    if (ContentSlot == nullptr) {
        return;
    }
    
    auto ContentWidget = Cast<UPanelWidget>(ContentSlot->GetContent());
    if (ContentWidget == nullptr) {
        return;
    }
#endif

    ClearSelectableOptions(false);
    for (int32 i = 0; i < ContentWidget->GetChildrenCount(); i++) {
        auto Child = Cast<URPGButtonBase>(ContentWidget->GetChildAt(i));
        Child->UpdateVisibility();
        if (!Child->IsVisible()) {
            continue;
        }

        AddOptionToWidget(Child);
    }

    if (GetIndex() != INDEX_NONE && GetIndex() >= GetItemCount()) {
        SetIndex(GetItemCount() - 1);
    }
}