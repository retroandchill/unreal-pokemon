//==========================================================================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FPaperFlipbookWidgetEditorModule : public IModuleInterface {
  public:
    /** IModuleInterface implementation */
    void StartupModule() override;
    void ShutdownModule() override;
};
