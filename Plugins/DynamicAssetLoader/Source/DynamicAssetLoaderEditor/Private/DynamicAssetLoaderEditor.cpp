#include "DynamicAssetLoaderEditor.h"
#include "Assets/AssetLoadingSettings.h"
#include "Details/VariantObjectCustomization.h"

void FDynamicAssetLoaderEditorModule::StartupModule() {
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(
        TEXT("AssetClassType"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&UE::Ranges::TVariantObjectCustomization<FAssetClassType>::MakeInstance));
}

void FDynamicAssetLoaderEditorModule::ShutdownModule() {
    // No shutdown needed
}
    
IMPLEMENT_MODULE(FDynamicAssetLoaderEditorModule, DynamicAssetLoaderEditor)