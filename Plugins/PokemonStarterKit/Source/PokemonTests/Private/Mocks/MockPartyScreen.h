// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <gmock/gmock.h>
#include "Screens/PartyScreen.h"
#include "UObject/Interface.h"

class FMockPartyScreen : public IPartyScreen {
public:
    ~FMockPartyScreen() override = default;

    MOCK_METHOD(void, BeginSwitch, (int32 Index), (override));
    MOCK_METHOD(void, ShowCommands, (const TArray<TObjectPtr<UPartyMenuHandler>> &Handlers), (override));
    MOCK_METHOD(void, ClearCommandStack, (), (override));
    MOCK_METHOD(void, SetCommandHelpText, (FText Text), (override));
    MOCK_METHOD(APlayerController *, GetPlayerController, (), (const, override));
    MOCK_METHOD(FOnPokemonSelected &, GetOnPokemonSelect, (), (override));
    MOCK_METHOD(void, RemoveFromStack, (), (override));
    MOCK_METHOD(void, RefreshScene, (), (override));
};