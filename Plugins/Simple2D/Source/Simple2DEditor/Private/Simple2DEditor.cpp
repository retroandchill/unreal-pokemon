#include "Simple2DEditor.h"
#include "AssetToolsModule.h"
#include "ComponentAssetBroker.h"
#include "Simple2D/Simple2DStyle.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookAssetActions.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookDetailsCustomization.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookThumbnailRenderer.h"

class IPaper2DEditorModule;

void FSimple2DEditorModule::StartupModule()
{
    FCoreDelegates::OnPostEngineInit.AddLambda([this] {
        FlipbookEditor_MenuExtensibilityManager = MakeShared<FExtensibilityManager>();
        FlipbookEditor_ToolBarExtensibilityManager = MakeShared<FExtensibilityManager>();
        
        // Register asset types
        IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.RegisterAssetTypeActions(MakeShared<Simple2D::FSimpleFlipbookAssetActions>());

        auto &PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.RegisterCustomClassLayout(USimpleFlipbook::StaticClass()->GetFName(),
            FOnGetDetailCustomizationInstance::CreateStatic(
                &Simple2D::FSimpleFlipbookDetailsCustomization::MakeInstance));

        UThumbnailManager::Get().RegisterCustomRenderer(USimpleFlipbook::StaticClass(), USimpleFlipbookThumbnailRenderer::StaticClass());

        Simple2D::FSimple2DStyle::Initialize();
    });
}

void FSimple2DEditorModule::ShutdownModule()
{
    FCoreDelegates::OnPostEngineInit.RemoveAll(this);

    FlipbookEditor_MenuExtensibilityManager.Reset();
    FlipbookEditor_ToolBarExtensibilityManager.Reset();

    Simple2D::FSimple2DStyle::Shutdown();
}
    
IMPLEMENT_MODULE(FSimple2DEditorModule, Simple2DEditor)