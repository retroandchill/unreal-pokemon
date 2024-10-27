// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

class FPopulateMenuBuilderWithToolMenuEntry {

public:
    static void ExecuteOption(const FToolMenuEntry& Entry) {
        Entry.Action.GetUIAction()->Execute();
    }
    
};