#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class FTilemap3DEditorModule : public IModuleInterface
{
public:
    void StartupModule() override;
    void ShutdownModule() override;

private:
    void OnPostEngineInit();

    EAssetTypeCategories::Type CategoryBit = EAssetTypeCategories::Basic;
};
