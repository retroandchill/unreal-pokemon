// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/SelectableOptionTestWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Primatives/SelectableOption.h"

void USelectableOptionTestWidget::CreateChildWidgets() {
    for (int32 i = 0; i < 3; i++) {
        auto NewWidget = WidgetTree->ConstructWidget<USelectableOption>(OptionWidget);
        NewWidget->SetOptionIndex(i);
        SlotCreatedWidget(NewWidget);
        NewWidget->GetOnOptionClicked().AddDynamic(this, &USelectableOptionTestWidget::ProcessClickedButton);
        NewWidget->GetOnOptionHovered().AddDynamic(this, &USelectableOptionTestWidget::ProcessHoveredButton);
        Options.Add(NewWidget);
    }
}

int32 USelectableOptionTestWidget::GetItemCount_Implementation() const {
    return Options.Num();
}

const TArray<TObjectPtr<USelectableOption>> &USelectableOptionTestWidget::GetOptions() const {
    return Options;
}