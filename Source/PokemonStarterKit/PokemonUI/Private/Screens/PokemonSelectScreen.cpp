// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PokemonSelectScreen.h"
#include "Components/Party/PokemonSelectionPane.h"
#include "Managers/PokemonSubsystem.h"
#include "Utilities/RPGMenuUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UPokemonSelectScreen)

UPokemonSelectScreen *UPokemonSelectScreen::AddPokemonSelectScreenToStack(const UObject *WorldContextObject)
{
    return URPGMenuUtilities::InjectScreenToStack<UPokemonSelectScreen>(WorldContextObject).GetPtrOrNull();
}

void UPokemonSelectScreen::NativeConstruct()
{
    Super::NativeConstruct();
    check(SelectionPane != nullptr)
    SelectionPane->SetIndex(0);
    SelectionPane->ActivateWidget();
}

void UPokemonSelectScreen::BeginSwitch(int32 Index)
{
    SelectionPane->ToggleCommandVisibility(true);

    SelectionPane->SetIndex(Index);
    SelectionPane->BeginSwitch(Index);
    SelectionPane->ActivateWidget();
}

APlayerController *UPokemonSelectScreen::GetPlayerController() const
{
    return GetOwningPlayer();
}

UE5Coro::TCoroutine<TOptional<FSelectedPokemonHandle>> UPokemonSelectScreen::PromptPokemonSelection()
{
    auto [Result] = co_await PokemonSelected;
    co_return Result;
}

void UPokemonSelectScreen::RefreshScene()
{
    SelectionPane->RefreshWindow();
}

void UPokemonSelectScreen::RefreshSelf_Implementation()
{
    Super::RefreshSelf_Implementation();
    RefreshScene();
}

UPokemonSelectionPane *UPokemonSelectScreen::GetSelectionPane() const
{
    return SelectionPane;
}

UPokemonSelectionPaneBase *UPokemonSelectScreen::GetPokemonSelectionPane_Implementation() const
{
    return SelectionPane;
}

void UPokemonSelectScreen::RemoveFromStack()
{
    CloseScreen();
}

void UPokemonSelectScreen::CloseScreen()
{
    Super::CloseScreen();
    (void)PokemonSelected.ExecuteIfBound({});
}

void UPokemonSelectScreen::OnPokemonSelected(int32 Index)
{
    auto Trainer = UPokemonSubsystem::GetInstance(this).GetPlayer();
    if (PokemonSelected.IsBound())
    {
        PokemonSelected.Execute(FSelectedPokemonHandle(this, Trainer, Index));
        return;
    }

    if (SelectionPane->IsSwitching())
    {
        if (int32 SwitchingIndex = SelectionPane->GetSwitchingIndex().GetValue(); Index != SwitchingIndex)
        {
            Trainer->SwapPositionsInParty(SwitchingIndex, Index);
        }
        SelectionPane->CompleteSwitch();
    }
    else
    {
        DisplayPokemonCommands(Trainer, Index);
    }
}

void UPokemonSelectScreen::OnPokemonCancel()
{
    if (SelectionPane->IsSwitching())
    {
        SelectionPane->CancelSwitch();
    }
    else
    {
        CloseScreen();
    }
}