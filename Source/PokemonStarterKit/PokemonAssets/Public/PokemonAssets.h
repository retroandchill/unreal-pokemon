// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * Editor module for handling the auto loading of Pokémon assets.
 */
class FPokemonAssetsModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
