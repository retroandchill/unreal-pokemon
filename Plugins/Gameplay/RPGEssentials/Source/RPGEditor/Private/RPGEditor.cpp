#include "RPGEditor.h"
#include "Details/RPGComponentInitializerCustomization.h"

#define LOCTEXT_NAMESPACE "FRPGEditorModule"

void FRPGEditorModule::StartupModule()
{
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    PropertyModule.RegisterCustomPropertyTypeLayout(
        "RPGComponentInitializer",
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FRPGComponentInitializerCustomization::MakeInstance));

    PropertyModule.NotifyCustomizationModuleChanged();
}

void FRPGEditorModule::ShutdownModule()
{
    auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    PropertyModule.UnregisterCustomPropertyTypeLayout("RPGComponentInitializer");

    PropertyModule.NotifyCustomizationModuleChanged();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRPGEditorModule, RPGEditor)