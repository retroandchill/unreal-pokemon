// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectionWidget.h"

#include "OptionSelectionWidget.generated.h"

class UDisplayTextOption;
class UInteractiveButtonBase;
class UCommonButtonGroupBase;
class UCommonButtonStyle;
class UCommonButtonBase;
class UInputAction;

USTRUCT(BlueprintType, meta = (NullableEnable, RecordStruct, ReadOnly, UseProperties))
struct FSelectableOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Selection", meta = (Required))
    FName Id;

    UPROPERTY(EditAnywhere, Category = "Selection")
    TOptional<FText> Text;

    UPROPERTY(EditAnywhere, Category = "Selection", meta = (Nullable))
    TObjectPtr<UInputAction> InputAction;
};

DECLARE_MULTICAST_DELEGATE_ThreeParams(FNativeOptionSelected, int32, FName, UInteractiveButtonBase *);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOptionSelected, int32, Index, FName, SelectedId,
                                               UInteractiveButtonBase *, Button);

/**
 *
 */
UCLASS(Abstract)
class INTERACTIVEUI_API UOptionSelectionWidget : public USelectionWidget
{
    GENERATED_BODY()

  protected:
    void NativePreConstruct() override;

  public:
    const TArray<FSelectableOption> &GetOptions() const
    {
        return Options;
    }

    void SetOptions(TArray<FSelectableOption> NewOptions);

#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext &Context) const override;
#endif

    FDelegateHandle BindToOptionSelected(FNativeOptionSelected::FDelegate Delegate);

    void UnbindFromOptionSelected(FDelegateHandle Handle);

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Selection")
    void SlotOption(int32 Index, FName Id, UInteractiveButtonBase *Button);

  private:
    void CreateOptions();

    UPROPERTY(EditAnywhere, Getter, Setter, Category = "Selection")
    TArray<FSelectableOption> Options;

    UPROPERTY(EditAnywhere, Category = "Selection")
    TSubclassOf<UDisplayTextOption> ButtonClass;

    UPROPERTY(EditAnywhere, Category = "Selection", meta = (InlineEditConditionToggle))
    bool bOverrideButtonStyle;

    UPROPERTY(EditAnywhere, Category = "Styles", meta = (EditCondition = "bOverrideButtonStyle"))
    TSubclassOf<UCommonButtonStyle> ButtonStyle;

    FNativeOptionSelected NativeOptionSelectedDelegate;

    UPROPERTY(BlueprintAssignable, Category = "Events", meta = (ScriptName = "OnOptionSelectedDelegate"))
    FOptionSelected OnOptionSelected;
};
