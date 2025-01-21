﻿#include "Simple2DEditor.h"
#include "AssetToolsModule.h"
#include "PaperFlipbook.h"
#include "../../Simple2D/Public/Simple2D"

class IPaper2DEditorModule;

void FSimple2DEditorModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddLambda([this] {
        // Register asset types
        IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.RegisterAssetTypeActions(MakeShared<FSimpleFlipbookAssetActions>());
    });
}

void FSimple2DEditorModule::ShutdownModule()
{
    // No special shutdown needed
}
    
IMPLEMENT_MODULE(FSimple2DEditorModule, Simple2DEditor)