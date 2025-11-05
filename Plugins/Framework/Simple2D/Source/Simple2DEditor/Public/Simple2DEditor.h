#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimple2DEditorModule : public IModuleInterface {
  public:
    void StartupModule() override;
    void ShutdownModule() override;

    TSharedPtr<FExtensibilityManager> GetFlipbookEditorMenuExtensibilityManager() {
        return FlipbookEditor_MenuExtensibilityManager;
    }
    TSharedPtr<FExtensibilityManager> GetFlipbookEditorToolBarExtensibilityManager() {
        return FlipbookEditor_ToolBarExtensibilityManager;
    }

  private:
    TSharedPtr<FExtensibilityManager> FlipbookEditor_MenuExtensibilityManager;
    TSharedPtr<FExtensibilityManager> FlipbookEditor_ToolBarExtensibilityManager;
};
