// "Unreal Pokémon" created by Retro & Chill.
#include "RPGMenusEditor.h"

#include "AssetToolsModule.h"
#include "Data/Windowskin.h"
#include "IAssetTools.h"
#include "Windowskin/WindowskinAssetActions.h"
#include "Windowskin/WindowskinThumbnailRenderer.h"

constexpr auto GLoctextNamespace = "FRPGMenusEditorModule";

void FRPGMenusEditorModule::StartupModule() {
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FRPGMenusEditorModule::OnPostEngineInit);
}

void FRPGMenusEditorModule::OnPostEngineInit() const {
    // Register asset types
    IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    AssetTools.RegisterAssetTypeActions(MakeShared<FWindowskinAssetActions>());

    // Register thumbnails
    UThumbnailManager::Get().RegisterCustomRenderer(UWindowskin::StaticClass(),
                                                    UWindowskinThumbnailRenderer::StaticClass());
}

void FRPGMenusEditorModule::ShutdownModule() {
    // No special shutdown needed
}

IMPLEMENT_MODULE(FRPGMenusEditorModule, RPGMenusEditor)
