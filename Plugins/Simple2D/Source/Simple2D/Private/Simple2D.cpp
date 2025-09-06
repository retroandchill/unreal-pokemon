// Copyright Epic Games, Inc. All Rights Reserved.

#include "Simple2D.h"
#include "Simple2D/Rendering/FlipbookRenderSection.h"

void FSimple2DModule::StartupModule()
{
    Simple2D::FSimpleFlipbookTangents::SetTangentsFromPaperAxes();
}

void FSimple2DModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

IMPLEMENT_MODULE(FSimple2DModule, Simple2D)