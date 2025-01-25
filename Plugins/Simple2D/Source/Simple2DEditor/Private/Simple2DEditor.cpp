#include "Simple2DEditor.h"
#include "AssetToolsModule.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Simple2D/Assets/SimpleFlipbookAssetActions.h"
#include "Simple2D/Assets/SimpleFlipbookDetailsCustomization.h"

class IPaper2DEditorModule;

void FSimple2DEditorModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddLambda([this] {
        // Register asset types
        IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.RegisterAssetTypeActions(MakeShared<Simple2D::FSimpleFlipbookAssetActions>());

        auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.RegisterCustomClassLayout(USimpleFlipbook::StaticClass()->GetFName(),
            FOnGetDetailCustomizationInstance::CreateStatic(
                &Simple2D::FSimpleFlipbookDetailsCustomization::MakeInstance));
    });
}

void FSimple2DEditorModule::ShutdownModule()
{
    // No special shutdown needed
}
    
IMPLEMENT_MODULE(FSimple2DEditorModule, Simple2DEditor)