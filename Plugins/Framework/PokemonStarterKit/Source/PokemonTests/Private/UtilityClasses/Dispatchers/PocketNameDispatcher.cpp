// "Unreal Pokémon" created by Retro & Chill.

#include "PocketNameDispatcher.h"

void UPocketNameDispatcher::OnReceivePocket(FName Pocket) {
    CurrentPocket = Pocket;
}