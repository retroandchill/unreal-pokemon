// "Unreal Pokémon" created by Retro & Chill.


#include "Components/SelectionWidget.h"
#include "Components/InteractiveButtonGroup.h"

void USelectionWidget::NativePreConstruct()
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

UWidget* USelectionWidget::NativeGetDesiredFocusTarget() const
{
    const int32 FocusIndex = DesiredFocusIndex.Get(INDEX_NONE);
    if (auto *ButtonAtIndex = Buttons->GetButtonBaseAtIndex(FocusIndex); ButtonAtIndex != nullptr)
    {
        return ButtonAtIndex;
    }
    
    return Super::NativeGetDesiredFocusTarget();
}

bool USelectionWidget::TryGetDesiredFocusIndex(int32& OutIndex) const
{
    if (DesiredFocusIndex.IsSet())
    {
        OutIndex = DesiredFocusIndex.GetValue();
        return true;
    }

    OutIndex = INDEX_NONE;
    return false;
}

void USelectionWidget::SetDesiredFocusIndex(const int32 Index)
{
    DesiredFocusIndex.Emplace(FMath::Clamp(Index, 0, Buttons->GetButtonCount() - 1));
}