#include "DynamicAssetLoaderEditor.h"
#include "Assets/AssetLoadingSettings.h"
#include "RetroLib/Variants/SoftVariantObjectCustomization.h"
#include "RetroLib/Variants/VariantObjectCustomization.h"

void FDynamicAssetLoaderEditorModule::StartupModule() {
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("AssetClassType"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(
                                    &Retro::TVariantObjectCustomization<FAssetClassType>::MakeInstance));
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("SoftAssetClassType"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(
                                        &Retro::TSoftVariantObjectCustomization<FSoftAssetClassType>::MakeInstance));
}

void FDynamicAssetLoaderEditorModule::ShutdownModule() {
    // No shutdown needed
}

IMPLEMENT_MODULE(FDynamicAssetLoaderEditorModule, DynamicAssetLoaderEditor)