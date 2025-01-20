#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimpleFlipbookEditorModule : public IModuleInterface {
public:
    void StartupModule() override;
    void ShutdownModule() override;
};
