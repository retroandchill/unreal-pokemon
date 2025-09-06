// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Bag/PocketTabWidget.h"
#include "Algo/ForEach.h"
#include "CommonActionWidget.h"
#include "Components/Bag/ItemSelectionWindow.h"
#include "Components/Bag/PocketButton.h"
#include "Engine/GameInstance.h"
#include "Groups/CommonButtonGroupBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Memory/CursorMemorySubsystem.h"
#include "PokemonDataSettings.h"
#include "RetroLib/Functional/BindMethod.h"
#include "RetroLib/Ranges/Algorithm/To.h"

UPocketTabWidget::UPocketTabWidget()
{
    for (auto &Pockets = GetDefault<UPokemonDataSettings>()->PocketNames; auto &[ID, Name] : Pockets)
    {
        PocketButtonStyles.Emplace(Name);
    }
}

void UPocketTabWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

#if WITH_EDITOR
    if (ButtonClass == nullptr)
    {
        return;
    }
#endif

    auto PocketNames = UItemHelper::GetPocketNames();
    Algo::ForEach(PocketButtons, &UWidget::RemoveFromParent);
    // clang-format off
    PocketButtons = PocketNames |
                    Retro::Ranges::Views::Transform(Retro::BindMethod<&UPocketTabWidget::CreatePocketButton>(this)) |
                    Retro::Ranges::To<TArray>();
    // clang-format on

    PocketLeftActionWidget->SetEnhancedInputAction(PocketLeftAction);
    PocketRightActionWidget->SetEnhancedInputAction(PocketRightAction);
}

void UPocketTabWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PocketButtonGroup = NewObject<UCommonButtonGroupBase>(this);

    for (int32 i = 0; i < PocketButtons.Num(); i++)
    {
        auto Button = PocketButtons[i];
        PocketButtonGroup->AddWidget(Button);
        Button->OnClicked().AddUObject(this, &UPocketTabWidget::OnPocketClicked, i);
    }

    auto CreateBindArgs = [this](UInputAction *Action, auto Function, bool bDisplayInActionBar = false) {
        FBindUIActionArgs BindArgs(Action, FSimpleDelegate::CreateUObject(this, Function));
        BindArgs.bDisplayInActionBar = bDisplayInActionBar;
        return BindArgs;
    };

    PocketLeftBinding = RegisterUIActionBinding(CreateBindArgs(PocketLeftAction, &UPocketTabWidget::PocketLeft));
    PocketRightBinding = RegisterUIActionBinding(CreateBindArgs(PocketRightAction, &UPocketTabWidget::PocketRight));

    auto &PocketNames = GetGameInstance()->GetSubsystem<UCursorMemorySubsystem>()->GetBagPocketNames();
    CurrentPocket = Retro::TCircularIterator<const FName>(PocketNames, 0);
    PocketButtons[CurrentPocket.GetIndex()]->SetIsSelected(true);
}

void UPocketTabWidget::SetItemSelectionWindow(UItemSelectionWindow *Window)
{
    ItemSelectionWindow = Window;
    ItemSelectionWindow->SetPocket(*CurrentPocket);
}

UPocketButton *UPocketTabWidget::CreatePocketButton(FName Pocket)
{
    auto Button = WidgetTree->ConstructWidget(ButtonClass);
    Button->SetPocket(Pocket);
    if (auto Style = PocketButtonStyles.FindChecked(Pocket); Style != nullptr)
    {
        Button->SetStyle(Style);
    }
    SlotButton(Button);
    return Button;
}

void UPocketTabWidget::PocketLeft()
{
    if (!ItemSelectionWindow->IsActivated())
    {
        return;
    }

    --CurrentPocket;
    PocketButtons[CurrentPocket.GetIndex()]->SetIsSelected(true);
    ItemSelectionWindow->SetPocket(*CurrentPocket);
}

void UPocketTabWidget::PocketRight()
{
    if (!ItemSelectionWindow->IsActivated())
    {
        return;
    }

    ++CurrentPocket;
    PocketButtons[CurrentPocket.GetIndex()]->SetIsSelected(true);
    ItemSelectionWindow->SetPocket(*CurrentPocket);
}

void UPocketTabWidget::OnPocketClicked(int32 Index)
{
    CurrentPocket.SetIndex(Index);
    PocketButtons[CurrentPocket.GetIndex()]->SetIsSelected(true);
    ItemSelectionWindow->SetPocket(*CurrentPocket);
}