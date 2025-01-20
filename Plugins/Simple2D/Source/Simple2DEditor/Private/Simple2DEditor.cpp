﻿#include "Simple2DEditor.h"
#include "AssetToolsModule.h"
#include "PaperFlipbook.h"
#include "SimpleFlipbook/Assets/SimpleFlipbookAssetActions.h"

class IPaper2DEditorModule;

void FSimple2DEditorModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddLambda([this] {
        // Register asset types
        IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        
        auto PaperFlipbookActions = AssetTools.GetAssetTypeActionsForClass(UPaperFlipbook::StaticClass());
        auto StrongPtr = PaperFlipbookActions.Pin();
        check(StrongPtr != nullptr);
        
        AssetTools.RegisterAssetTypeActions(MakeShared<FSimpleFlipbookAssetActions>(StrongPtr->GetTypeColor(), StrongPtr->GetCategories()));
    });
}

void FSimple2DEditorModule::ShutdownModule()
{
    // No special shutdown needed
}
    
IMPLEMENT_MODULE(FSimple2DEditorModule, Simple2DEditor)