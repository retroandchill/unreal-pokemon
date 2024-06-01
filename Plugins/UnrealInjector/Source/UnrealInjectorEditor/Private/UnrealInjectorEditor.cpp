#include "UnrealInjectorEditor.h"
#include "Details/InjectionTargetCustomization.h"

#define LOCTEXT_NAMESPACE "FUnrealInjectorEditorModule"

void FUnrealInjectorEditorModule::StartupModule() {
    auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("InjectionTarget"),
        FOnGetPropertyTypeCustomizationInstance::CreateLambda(&FInjectionTargetCustomization::MakeInstance));
}

void FUnrealInjectorEditorModule::ShutdownModule() {
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FUnrealInjectorEditorModule, UnrealInjectorEditor)