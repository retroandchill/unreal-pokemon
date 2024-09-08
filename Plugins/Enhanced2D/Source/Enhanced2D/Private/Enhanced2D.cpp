// Copyright Epic Games, Inc. All Rights Reserved.

#include "Enhanced2D.h"

#define LOCTEXT_NAMESPACE "FEnhanced2DModule"

void FEnhanced2DModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FEnhanced2DModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEnhanced2DModule, Enhanced2D)