// "Unreal Pokémon" created by Retro & Chill.

#include "Components/NamedSlotSelectableWidget.h"
#include "CommonButtonBase.h"
#include "Components/NamedSlot.h"
#include "Components/InteractiveButtonBase.h"
#include "Components/InteractiveButtonGroup.h"

void UNamedSlotSelectableWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    UpdateOptions();
}

void UNamedSlotSelectableWidget::UpdateOptions()
{
#if WITH_EDITOR
    if (ContentSlot == nullptr)
    {
        return;
    }
#endif

    const auto *ContentWidget = Cast<UPanelWidget>(ContentSlot->GetContent());
#if WITH_EDITOR
    if (ContentWidget == nullptr)
    {
        return;
    }
#endif

    GetButtons()->RemoveAll();
    for (int32 i = 0; i < ContentWidget->GetChildrenCount(); i++)
    {
        auto *Child = Cast<UInteractiveButtonBase>(ContentWidget->GetChildAt(i));
        if (Child == nullptr)
        {
            continue;
        }

        Child->OnClicked().Clear();
        Child->OnHovered().Clear();

        Child->UpdateVisibility();
        if (!Child->IsVisible())
        {
            continue;
        }

        GetButtons()->AddWidget(Child);
    }

    if (int32 Index; TryGetDesiredFocusIndex(Index) && Index >= GetButtons()->GetButtonCount())
    {
        SetDesiredFocusIndex(GetButtons()->GetButtonCount() - 1);
    }
}