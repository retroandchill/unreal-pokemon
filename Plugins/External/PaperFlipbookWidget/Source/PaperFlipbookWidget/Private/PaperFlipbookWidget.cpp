//==========================================================================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "PaperFlipbookWidget.h"

#define LOCTEXT_NAMESPACE "PaperFlipbookWidgetModule"

void FPaperFlipbookWidgetModule::StartupModule() {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin
    // file per-module
}

void FPaperFlipbookWidgetModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPaperFlipbookWidgetModule, PaperFlipbookWidget)