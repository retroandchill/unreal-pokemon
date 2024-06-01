#include "UnrealInjectorEditor.h"
#include "Details/InjectionTargetCustomization.h"

void FUnrealInjectorEditorModule::StartupModule() {
    auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("InjectionTarget"),
        FOnGetPropertyTypeCustomizationInstance::CreateLambda(&FInjectionTargetCustomization::MakeInstance));
}

void FUnrealInjectorEditorModule::ShutdownModule() {
    // No shutdown code needed    
}
    
IMPLEMENT_MODULE(FUnrealInjectorEditorModule, UnrealInjectorEditor)