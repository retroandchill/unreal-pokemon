﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Mocking/UnrealMock.h"
#include "Screens/PartyScreen.h"
#include "UObject/Interface.h"
#include <gmock/gmock.h>

class FMockPartyScreen : public IPartyScreen {
    DECLARE_MOCK_INTERFACE(FMockPartyScreen)

  public:
    MOCK_METHOD(void, BeginSwitch, (int32 Index), (override));
    MOCK_METHOD(APlayerController *, GetPlayerController, (), (const, override));
    MOCK_METHOD(FOnPokemonSelected &, GetOnPokemonSelect, (), (override));
    MOCK_METHOD(void, RemoveFromStack, (), (override));
    MOCK_METHOD(void, RefreshScene, (), (override));
};