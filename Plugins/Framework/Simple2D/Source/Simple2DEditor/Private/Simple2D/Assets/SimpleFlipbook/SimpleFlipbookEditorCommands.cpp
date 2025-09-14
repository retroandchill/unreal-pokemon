// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditorCommands.h"

namespace Simple2D
{
    void FSimpleFlipbookEditorCommands::RegisterCommands()
    {
        constexpr auto TEXT(LOCTEXT_NAMESPACE) = TEXT("Simple2D");

        UI_COMMAND(AddKeyFrame, "Add Key Frame", "Inserts a new key frame at the current time",
                   EUserInterfaceActionType::Button, FInputChord())

        UI_COMMAND(SetShowGrid, "Grid", "Displays the viewport grid.", EUserInterfaceActionType::ToggleButton,
                   FInputChord())
        UI_COMMAND(SetShowBounds, "Bounds", "Toggles display of the bounds of the static mesh.",
                   EUserInterfaceActionType::ToggleButton, FInputChord())
        UI_COMMAND(SetShowPivot, "Show Pivot", "Display the pivot location of the static mesh.",
                   EUserInterfaceActionType::ToggleButton, FInputChord())

        UI_COMMAND(AddNewFrame, "Add Key Frame", "Adds a new key frame to the flipbook.",
                   EUserInterfaceActionType::Button, FInputChord())
        UI_COMMAND(AddNewFrameBefore, "Insert Key Frame Before",
                   "Adds a new key frame to the flipbook before the selection.", EUserInterfaceActionType::Button,
                   FInputChord())
        UI_COMMAND(AddNewFrameAfter, "Insert Key Frame After",
                   "Adds a new key frame to the flipbook after the selection.", EUserInterfaceActionType::Button,
                   FInputChord())
    }
} // namespace Simple2D