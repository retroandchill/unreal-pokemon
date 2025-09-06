// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"

#include "SummaryTabWidget.generated.h"

class USummaryScreenPage;
class USummaryPages;
class UCommonButtonStyle;
class UCommonButtonBase;
class UCommonButtonGroupBase;
class UCommonActionWidget;
class UInputAction;

/**
 *
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryTabWidget : public UCommonUserWidget
{
    GENERATED_BODY()

  protected:
    void NativePreConstruct() override;
    void NativeConstruct() override;

  public:
    void SetSummaryPages(USummaryPages *Window);

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Layout)
    void SlotTabButton(UCommonButtonBase *Button);

  private:
    UCommonButtonBase *CreatePageButton(USummaryScreenPage *Page);

    void PageLeft() const;
    void PageRight() const;
    void OnPageClicked(int32 Index) const;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonActionWidget> PageLeftActionWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonActionWidget> PageRightActionWidget;

    UPROPERTY(EditAnywhere, Category = Appearance)
    TSubclassOf<UCommonButtonBase> PageButtonClass;

    UPROPERTY()
    TArray<TObjectPtr<UCommonButtonBase>> PageButtons;

    UPROPERTY()
    TObjectPtr<USummaryPages> SummaryPages;

    UPROPERTY()
    TObjectPtr<UCommonButtonGroupBase> PageButtonGroup;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> PageLeftAction;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> PageRightAction;

    FUIActionBindingHandle PageLeftBinding;
    FUIActionBindingHandle PageRightBinding;
};
