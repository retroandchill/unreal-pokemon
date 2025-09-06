// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealInjector.h"
#include "DependencyInjectionSettings.h"

#define LOCTEXT_NAMESPACE "FUnrealInjectorModule"

void FUnrealInjectorModule::StartupModule()
{
    // No startup code
}

void FUnrealInjectorModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealInjectorModule, UnrealInjector)