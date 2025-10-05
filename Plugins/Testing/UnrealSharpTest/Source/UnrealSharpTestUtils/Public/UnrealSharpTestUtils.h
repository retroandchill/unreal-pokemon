#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUnrealSharpTestUtils, Log, All);

/**
 * @class FUnrealSharpTestUtilsModule
 * @brief The module responsible for handling the initialization and cleanup
 *        of the UnrealSharpTestUtils module.
 *
 * This class implements the IModuleInterface to manage the startup and
 * shutdown lifecycle of the UnrealSharpTestUtils module.
 */
class FUnrealSharpTestUtilsModule : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;
};
