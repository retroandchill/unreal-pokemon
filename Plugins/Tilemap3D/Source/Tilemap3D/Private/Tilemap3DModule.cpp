// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tilemap3DModule.h"

void FTilemap3DModule::StartupModule() {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin
    // file per-module
}

void FTilemap3DModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

IMPLEMENT_MODULE(FTilemap3DModule, Tilemap3D)