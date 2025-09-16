// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OptionSelectionWidget.h"
#include "Components/DisplayTextOption.h"
#include "Components/InteractiveButtonBase.h"
#include "Components/InteractiveButtonGroup.h"


#include "Groups/CommonButtonGroupBase.h"
#include "Misc/DataValidation.h"

void UOptionSelectionWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    GetButtons()->BindToPlaceButton(FNativeAddButton::FDelegate::CreateWeakLambda(this, [this](const int32 Index, UCommonButtonBase* Button)
    {
        SlotOption(Index, Options[Index].Id, CastChecked<UInteractiveButtonBase>(Button));
    }));
    GetButtons()->NativeOnButtonBaseClicked.AddWeakLambda(this, [this](UCommonButtonBase* Button, const int32 Index)
    {
        const auto& [Id, Text, InputAction] = Options[Index];
        auto *NewButton = CastChecked<UInteractiveButtonBase>(Button);
        NativeOptionSelectedDelegate.Broadcast(Index, Id, NewButton);
        OnOptionSelected.Broadcast(Index, Id, NewButton);
    });
    GetButtons()->BindToRemoveButton(FNativeRemoveButton::FDelegate::CreateLambda([](UCommonButtonBase* Button)
    {
        Button->RemoveFromParent();
    }));
    
    CreateOptions();
}

void UOptionSelectionWidget::SetOptions(TArray<FSelectableOption> NewOptions)
{
    Options = MoveTemp(NewOptions);
    CreateOptions();
}

#if WITH_EDITOR
EDataValidationResult UOptionSelectionWidget::IsDataValid(FDataValidationContext& Context) const
{
    auto OriginalResult = Super::IsDataValid(Context);

    if (ButtonClass == nullptr)
    {
        Context.AddError(NSLOCTEXT("TurnBasedUI", "OptionSelectionWidget_ButtonClass_Error", "Button class is not set"));
        OriginalResult = EDataValidationResult::Invalid;
    }

    if (bOverrideButtonStyle && !IsValid(ButtonStyle))
    {
        Context.AddError(NSLOCTEXT("TurnBasedUI", "OptionSelectionWidget_ButtonStyle_Error", "Button style is not set"));
        OriginalResult = EDataValidationResult::Invalid;   
    } 

    return OriginalResult;
}
#endif

FDelegateHandle UOptionSelectionWidget::BindToOptionSelected(FNativeOptionSelected::FDelegate Delegate)
{
    return NativeOptionSelectedDelegate.Add(MoveTemp(Delegate));
}

void UOptionSelectionWidget::UnbindFromOptionSelected(const FDelegateHandle Handle)
{
    NativeOptionSelectedDelegate.Remove(Handle);   
}

void UOptionSelectionWidget::CreateOptions()
{
    GetButtons()->RemoveAll();
    for (int32 i = 0; i < Options.Num(); ++i)
    {
        auto &[Id, Text, InputAction] = Options[i];
        auto *NewButton = CreateWidget<UDisplayTextOption>(this, ButtonClass);
        if (Text.IsSet())
        {
            NewButton->SetText(*Text);
        }

        if (InputAction != nullptr)
        {
            NewButton->SetTriggeringEnhancedInputAction(InputAction);
        }

        if (bOverrideButtonStyle && ensure(ButtonStyle != nullptr))
        {
            NewButton->SetStyle(ButtonStyle);
        }

        GetButtons()->AddWidget(NewButton);
    }
}
