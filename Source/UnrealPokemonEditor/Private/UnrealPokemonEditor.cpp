#include "UnrealPokemonEditor.h"
#include "AnimSequences/PaperZDAnimSequence_Flipbook.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Thumbnails/PaperZDAnimSequenceFlipbookThumbnailRenderer.h"

DEFINE_LOG_CATEGORY(LogUnrealPokemonEditor)

void FUnrealPokemonEditorModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FUnrealPokemonEditorModule::OnPostEngineInit);
    // No startup procedure
}

void FUnrealPokemonEditorModule::ShutdownModule()
{
    // No shutdown needed
}

void FUnrealPokemonEditorModule::OnPostEngineInit() const
{
    UThumbnailManager::Get().RegisterCustomRenderer(UPaperZDAnimSequence_Flipbook::StaticClass(),
                                                    UPaperZDAnimSequenceFlipbookThumbnailRenderer::StaticClass());
}

IMPLEMENT_MODULE(FUnrealPokemonEditorModule, UnrealPokemonEditor)