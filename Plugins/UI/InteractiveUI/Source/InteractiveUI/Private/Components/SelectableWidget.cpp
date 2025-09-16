// "Unreal Pokémon" created by Retro & Chill.
#include "Components/SelectableWidget.h"
#include "CommonButtonBase.h"
#include "Components/InteractiveButtonGroup.h"
#include "Groups/CommonButtonGroupBase.h"

void USelectableWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

#if WITH_EDITOR
    if (Buttons != nullptr)
    {
        Buttons->RemoveAll();
    }
#endif
    
    Buttons = NewObject<UInteractiveButtonGroup>(this);

    Buttons->NativeOnButtonBaseClicked.AddWeakLambda(this, [this](UCommonButtonBase*, const int32 Index)
    {
        DesiredFocusIndex = Index;
    });
}

UWidget* USelectableWidget::NativeGetDesiredFocusTarget() const
{
    const int32 FocusIndex = DesiredFocusIndex.Get(INDEX_NONE);
    if (auto *ButtonAtIndex = Buttons->GetButtonBaseAtIndex(FocusIndex); ButtonAtIndex != nullptr)
    {
        return ButtonAtIndex;
    }
    
    return Super::NativeGetDesiredFocusTarget();
}

bool USelectableWidget::TryGetDesiredFocusIndex(int32& OutIndex) const
{
    if (DesiredFocusIndex.IsSet())
    {
        OutIndex = DesiredFocusIndex.GetValue();
        return true;
    }

    OutIndex = INDEX_NONE;
    return false;
}

void USelectableWidget::SetDesiredFocusIndex(const int32 Index)
{
    DesiredFocusIndex.Emplace(FMath::Clamp(Index, 0, Buttons->GetButtonCount() - 1));
}

int32 USelectableWidget::GetIndex() const
{
    return GetDesiredFocusIndex().Get(INDEX_NONE);
}

void USelectableWidget::SetIndex(int32 NewIndex)
{
    SetDesiredFocusIndex(NewIndex);
}

void USelectableWidget::Deselect()
{
    ClearDesiredFocusIndex();
}

UCommonButtonBase * USelectableWidget::GetSelectedOption() const
{
    return GetSelectableOption(GetIndex());
}

UCommonButtonBase * USelectableWidget::GetSelectableOption(int32 OptionIndex) const
{
    return Buttons->GetButtonBaseAtIndex(OptionIndex);
}

void USelectableWidget::ClearSelectableOptions(const bool bRemoveWidgets)
{
    if (bRemoveWidgets)
    {
        Buttons->ForEach([](UCommonButtonBase &Button, int32) {
            Button.RemoveFromParent();
        });
    }

    Buttons->RemoveAll();
}

void USelectableWidget::SlotOption(UCommonButtonBase *Option)
{
    int32 OptionIndex = AddOptionToWidget(Option);
    PlaceOptionIntoWidget(Option, OptionIndex);
}

int32 USelectableWidget::AddOptionToWidget(UCommonButtonBase *Option)
{
    Buttons->AddWidget(Option);
    return Buttons->GetButtonCount() - 1;
}

void USelectableWidget::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex)
{
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::ProcessConfirm_Implementation(int32 CurrentIndex)
{
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::ProcessCancel_Implementation()
{
    // No implementation, but we cannot have an abstract method in an Unreal class
}
