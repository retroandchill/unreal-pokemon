// "Unreal Pokémon" created by Retro & Chill.
#include "RPGMenusEditor.h"
#include "AssetToolsModule.h"
#include "Data/Windowskin.h"
#include "IAssetTools.h"
#include "Images/ImageAsset.h"
#include "RetroLib/Variants/SoftVariantObjectCustomization.h"
#include "RetroLib/Variants/VariantObjectCustomization.h"
#include "Windowskin/WindowskinAssetActions.h"
#include "Windowskin/WindowskinThumbnailRenderer.h"

constexpr auto GLoctextNamespace = "FRPGMenusEditorModule";

void FRPGMenusEditorModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FRPGMenusEditorModule::OnPostEngineInit);
}

void FRPGMenusEditorModule::OnPostEngineInit() const
{
    // Register asset types
    IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    AssetTools.RegisterAssetTypeActions(MakeShared<FWindowskinAssetActions>());

    // Register thumbnails
    UThumbnailManager::Get().RegisterCustomRenderer(UWindowskin::StaticClass(),
                                                    UWindowskinThumbnailRenderer::StaticClass());

    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("ImageAsset"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(
                                &Retro::TVariantObjectCustomization<FImageAsset>::MakeInstance));

    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("SoftImageAsset"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(
                                    &Retro::TSoftVariantObjectCustomization<FSoftImageAsset>::MakeInstance));
}

void FRPGMenusEditorModule::ShutdownModule()
{
    // No special shutdown needed
}

IMPLEMENT_MODULE(FRPGMenusEditorModule, RPGMenusEditor)
