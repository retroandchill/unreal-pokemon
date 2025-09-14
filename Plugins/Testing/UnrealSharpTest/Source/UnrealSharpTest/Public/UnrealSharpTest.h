// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FCSharpAutomationTest;
class FManagedTestHandle;

DECLARE_LOG_CATEGORY_EXTERN(LogUnrealSharpTestNative, Log, All);

/**
 * @class FUnrealSharpTestModule
 * @brief This class represents a module in the Unreal Engine project, designed for integrating
 *        UnrealSharp features or testing specific functionalities.
 *
 * The FUnrealSharpTestModule class is typically used for module-level initialization and teardown
 * in the context of Unreal Engine's module management. It provides hooks into the engine for performing
 * custom logic during the module's startup and shutdown phases.
 *
 * This class is primarily intended for Unreal Engine developers who are extending the engine's
 * capabilities or integrating external libraries into the system as a module.
 *
 * Features provided by this module can include additional testing utilities, support for new
 * functionality, or extended compatibility for other components or plugins.
 *
 * Responsibilities:
 * - Handles initialization of resources particular to the module.
 * - Ensures cleanup after the module's lifecycle ends.
 * - Provides hooks for Unreal Engine's module framework to allow seamless integration.
 *
 * Usage Restrictions:
 * This class is specific to Unreal Engine-based projects and presupposes familiarity with
 * Unreal Engine's module concept and API framework.
 */
class FUnrealSharpTestModule final : public IModuleInterface
{
  public:
    void StartupModule() override;
    void ShutdownModule() override;

    /**
     * @brief Provides access to the singleton instance of FUnrealSharpTestModule.
     *
     * This method returns a reference to the statically stored singleton instance
     * of the FUnrealSharpTestModule class. It ensures that the caller can access
     * the module's unique instance for managing module-specific operations.
     *
     * @return A reference to the singleton instance of FUnrealSharpTestModule.
     */
    static FUnrealSharpTestModule &Get()
    {
        return *Instance;
    }

  private:
    void RegisterTests();
    void RegisterTests(const FName &AssemblyName);
    void RegisterTests(TConstArrayView<FName, int> Assemblies);
    void UnregisterTests(const FName &AssemblyName);

    static FUnrealSharpTestModule *Instance;
    TMap<FName, TArray<TSharedRef<FCSharpAutomationTest>>> Tests;
    FDelegateHandle RegisterTestsDelegateHandle;
    FDelegateHandle ClearTestCacheDelegateHandle;
};
