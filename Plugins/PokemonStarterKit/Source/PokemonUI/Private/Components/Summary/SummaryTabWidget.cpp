// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/SummaryTabWidget.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "CommonActionWidget.h"
#include "Components/Summary/SummaryPages.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Groups/CommonButtonGroupBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Ranges/Algorithm/ToArray.h"

void USummaryTabWidget::NativePreConstruct() {
    Super::NativePreConstruct();

    PageLeftActionWidget->SetEnhancedInputAction(PageLeftAction);
    PageRightActionWidget->SetEnhancedInputAction(PageRightAction);
}

void USummaryTabWidget::NativeConstruct() {
    Super::NativeConstruct();

    auto CreateBindArgs = [this](UInputAction *Action, auto Function, bool bDisplayInActionBar = false) {
        FBindUIActionArgs BindArgs(Action, FSimpleDelegate::CreateUObject(this, Function));
        BindArgs.bDisplayInActionBar = bDisplayInActionBar;
        return BindArgs;
    };

    PageLeftBinding = RegisterUIActionBinding(CreateBindArgs(PageLeftAction, &USummaryTabWidget::PageLeft));
    PageRightBinding = RegisterUIActionBinding(CreateBindArgs(PageRightAction, &USummaryTabWidget::PageRight));
}

void USummaryTabWidget::SetSummaryPages(USummaryPages *Window) {
    SummaryPages = Window;

#if WITH_EDITOR
    if (PageButtonClass == nullptr) {
        return;
    }
#endif

    PageButtonGroup = NewObject<UCommonButtonGroupBase>(this);
    Algo::ForEach(PageButtons, &UWidget::RemoveFromParent);

    auto Pages = SummaryPages->GetPages();
    PageButtons = Pages | ranges::views::transform(std::bind_front(&USummaryTabWidget::CreatePageButton, this)) |
                  UE::Ranges::ToArray;

    PageButtons[SummaryPages->GetCurrentPageIndex()]->SetIsSelected(true);
}

UCommonButtonBase *USummaryTabWidget::CreatePageButton(USummaryScreenPage *Page) {
    auto Button = WidgetTree->ConstructWidget(PageButtonClass);
    Button->SetStyle(Page->GetTabButtonStyle());
    PageButtonGroup->AddWidget(Button);
    Button->OnClicked().AddUObject(this, &USummaryTabWidget::OnPageClicked, PageButtons.Num());
    SlotTabButton(Button);
    PageButtons.Emplace(Button);
    return Button;
}

void USummaryTabWidget::PageLeft() const {
    SummaryPages->PreviousPage();
    PageButtons[SummaryPages->GetCurrentPageIndex()]->SetIsSelected(true);
}

void USummaryTabWidget::PageRight() const {
    SummaryPages->NextPage();
    PageButtons[SummaryPages->GetCurrentPageIndex()]->SetIsSelected(true);
}

void USummaryTabWidget::OnPageClicked(int32 Index) const {
    SummaryPages->SetPage(Index);
    PageButtons[SummaryPages->GetCurrentPageIndex()]->SetIsSelected(true);
}