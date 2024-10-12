#include "DynamicAssetLoaderEditor.h"
#include "Assets/AssetLoadingSettings.h"
#include "Details/SoftVariantObjectCustomization.h"
#include "Details/VariantObjectCustomization.h"

void FDynamicAssetLoaderEditorModule::StartupModule() {
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("AssetClassType"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&UE::Ranges::TVariantObjectCustomization<FAssetClassType>::MakeInstance));
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("SoftAssetClassType"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&UE::Ranges::TSoftVariantObjectCustomization<FSoftAssetClassType>::MakeInstance));
}

void FDynamicAssetLoaderEditorModule::ShutdownModule() {
    // No shutdown needed
}
    
IMPLEMENT_MODULE(FDynamicAssetLoaderEditorModule, DynamicAssetLoaderEditor)