// Copyright Epic Games, Inc. All Rights Reserved.

#ifdef __UNREAL__
#include "RetroLibModule.h"

void FRetroLibModule::StartupModule() {
    // No special startup code
}

void FRetroLibModule::ShutdownModule() {
    // No special shutdown code
}

IMPLEMENT_MODULE(FRetroLibModule, RetroLib)
#endif