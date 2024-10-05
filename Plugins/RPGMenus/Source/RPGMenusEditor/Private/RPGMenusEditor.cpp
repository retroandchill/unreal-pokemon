// "Unreal Pokémon" created by Retro & Chill.
#include "RPGMenusEditor.h"
#include "AssetToolsModule.h"
#include "Data/Windowskin.h"
#include "IAssetTools.h"
#include "Details/VariantObjectCustomization.h"
#include "Windowskin/WindowskinAssetActions.h"
#include "Windowskin/WindowskinThumbnailRenderer.h"
#include "Images/ImageAsset.h"

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
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("ImageAsset"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&UE::Ranges::TVariantObjectCustomization<FImageAsset>::MakeInstance));
}

void FRPGMenusEditorModule::ShutdownModule() {
    // No special shutdown needed
}

IMPLEMENT_MODULE(FRPGMenusEditorModule, RPGMenusEditor)
