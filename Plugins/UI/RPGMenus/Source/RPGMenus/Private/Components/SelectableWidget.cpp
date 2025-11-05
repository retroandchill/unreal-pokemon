// "Unreal Pokémon" created by Retro & Chill.
#include "Components/SelectableWidget.h"
#include "Algo/ForEach.h"
#include "CommonButtonBase.h"
#include "Groups/CommonButtonGroupBase.h"
#include "RetroLib/Optionals/IfPresent.h"

USelectableWidget::USelectableWidget(const FObjectInitializer &Initializer) : UCommonActivatableWidget(Initializer) {
    bIsBackHandler = true;
    bIsBackActionDisplayedInActionBar = true;
}

void USelectableWidget::NativePreConstruct() {
    Super::NativePreConstruct();
    SelectableButtonGroup = NewObject<UCommonButtonGroupBase>(this);
}

int32 USelectableWidget::GetItemCount() const {
    return SelectableButtons.Num();
}

int32 USelectableWidget::GetIndex() const {
    return Index;
}

void USelectableWidget::SetIndex(int32 NewIndex) {
    int32 OldIndex = Index;
    Index = FMath::Clamp(NewIndex, static_cast<int32>(INDEX_NONE), GetItemCount() - 1);
    Retro::Optionals::OfNullable(GetSelectableOption(Index)) |
        Retro::Optionals::IfPresent(Retro::BindBack<&UCommonButtonBase::SetIsSelected>(true, false));
    OnSelectionChange(OldIndex, Index);
}

void USelectableWidget::Deselect() {
    int32 OldIndex = Index;
    Index = INDEX_NONE;
    Retro::Optionals::OfNullable(GetSelectableOption(OldIndex)) |
        Retro::Optionals::IfPresent(Retro::BindBack<&UCommonButtonBase::SetIsSelected>(false, false));
    OnSelectionChange(OldIndex, Index);
}

UCommonButtonBase *USelectableWidget::GetSelectedOption() const {
    if (!SelectableButtons.IsValidIndex(Index)) {
        return nullptr;
    }

    return SelectableButtons[Index];
}

UE5Coro::TCoroutine<TOptional<int32>> USelectableWidget::AwaitSelection() {
    auto Result = co_await Race(
        [](UE5Coro::TLatentContext<USelectableWidget> This) -> UE5Coro::TCoroutine<int32> {
            auto [Result] = co_await This->OnConfirm;
            co_return Result;
        }(this),
        [](UE5Coro::TLatentContext<USelectableWidget> This) -> UE5Coro::TCoroutine<> {
            co_await This->OnCancel;
        }(this));

    if (Result != 0) {
        co_return {};
    }

    co_return Index;
}

FProcessConfirm &USelectableWidget::GetOnConfirm() {
    return OnConfirm;
}

FProcessCancel &USelectableWidget::GetOnCancel() {
    return OnCancel;
}

UWidget *USelectableWidget::NativeGetDesiredFocusTarget() const {
    if (!SelectableButtons.IsValidIndex(Index)) {
        return nullptr;
    }

    return SelectableButtons[Index];
}

void USelectableWidget::NativeOnActivated() {
    Super::NativeOnActivated();
    if (GetVisibility() == ESlateVisibility::HitTestInvisible) {
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

void USelectableWidget::NativeOnDeactivated() {
    Super::NativeOnDeactivated();
    if (IsVisible()) {
        SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}

void USelectableWidget::ConfirmOnIndex(int32 CurrentIndex) {
    OnConfirm.Broadcast(CurrentIndex);
    ProcessConfirm(CurrentIndex);
}

void USelectableWidget::Cancel() {
    OnCancel.Broadcast();
    ProcessCancel();
}

bool USelectableWidget::NativeOnHandleBackAction() {
    Cancel();
    return true;
}

UCommonButtonBase *USelectableWidget::GetSelectableOption(int32 OptionIndex) const {
    if (!SelectableButtons.IsValidIndex(OptionIndex)) {
        return nullptr;
    }

    return SelectableButtons[OptionIndex];
}

void USelectableWidget::ClearSelectableOptions(bool bRemoveWidgets) {
    if (bRemoveWidgets) {
        Algo::ForEach(SelectableButtons, &UWidget::RemoveFromParent);
    }

    SelectableButtons.Reset();
    SelectableButtonGroup->RemoveAll();
}

void USelectableWidget::SlotOption(UCommonButtonBase *Option) {
    int32 OptionIndex = AddOptionToWidget(Option);
    PlaceOptionIntoWidget(Option, OptionIndex);
}

int32 USelectableWidget::AddOptionToWidget(UCommonButtonBase *Option) {
    int32 OptionIndex = SelectableButtons.Emplace(Option);
    Option->OnClicked().AddWeakLambda(this, [this, OptionIndex] {
        if (!IsActivated()) {
            return;
        }

        if (!bSelectOptionOnHover) {
            SetIndex(OptionIndex);
        }
        ConfirmOnIndex(OptionIndex);
    });
    Option->OnHovered().AddWeakLambda(this, [this, OptionIndex] {
        if (IsActivated() && bSelectOptionOnHover) {
            SetIndex(OptionIndex);
        }
    });
    SelectableButtonGroup->AddWidget(Option);
    return OptionIndex;
}

void USelectableWidget::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::ProcessConfirm_Implementation(int32 CurrentIndex) {
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::ProcessCancel_Implementation() {
    // No implementation, but we cannot have an abstract method in an Unreal class
}
