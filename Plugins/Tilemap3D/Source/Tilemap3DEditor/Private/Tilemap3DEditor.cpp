#include "Tilemap3DEditor.h"
#include "Assets/Tileset3DAssetTypeActions.h"
#include "AssetToolsModule.h"

class FAssetToolsModule;

void FTilemap3DEditorModule::StartupModule() {
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FTilemap3DEditorModule::OnPostEngineInit);
}

void FTilemap3DEditorModule::ShutdownModule() {
}

void FTilemap3DEditorModule::OnPostEngineInit() {
    auto &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    CategoryBit = AssetTools.RegisterAdvancedAssetCategory(
        FName(TEXT("Tilemap3D")), NSLOCTEXT("Tilemap3D", "Tilemap3DAssetCategory", "3D Tilemap"));

    AssetTools.RegisterAssetTypeActions(MakeShared<FTileset3DAssetTypeActions>(CategoryBit));
}

IMPLEMENT_MODULE(FTilemap3DEditorModule, Tilemap3DEditor)