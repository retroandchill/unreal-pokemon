// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/MoveInfoWindow.h"
#include "Pokemon/Moves/Move.h"

void UMoveInfoWindow::SetMove(const TScriptInterface<IMove> &NewMove)
{
    Move = NewMove;
    if (IsValid(Move.GetObject()))
    {
        OnMoveSet(NewMove);
    }
    else
    {
        OnMoveCleared();
    }
}