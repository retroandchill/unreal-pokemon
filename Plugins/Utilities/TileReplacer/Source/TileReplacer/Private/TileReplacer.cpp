#include "TileReplacer.h"

#define LOCTEXT_NAMESPACE "FTileReplacerModule"

void FTileReplacerModule::StartupModule() {
    // No startup code required
}

void FTileReplacerModule::ShutdownModule() {
    // No shutdown code required
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTileReplacerModule, TileReplacer)