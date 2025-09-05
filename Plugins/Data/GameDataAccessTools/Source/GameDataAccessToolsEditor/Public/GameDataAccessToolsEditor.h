#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Serialization/GameDataEntrySerializer.h"

class FGameDataAccessToolsEditorModule;
class FGameDataRepositoryActions;

class GAMEDATAACCESSTOOLSEDITOR_API IGameDataAccessToolsEditorModule : public IModuleInterface
{
public:
    static IGameDataAccessToolsEditorModule& Get();

    virtual TArray<FGameDataEntrySerializerPtr> GetAvailableSerializers(TSubclassOf<UGameDataRepository> RepositoryClass) const = 0;
};
