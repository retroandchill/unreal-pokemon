// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/ModuleManager.h"

/**
 * Implements the FCommonGameModule module.
 */
class FCommonGameModule : public IModuleInterface {
  public:
    FCommonGameModule();
    void StartupModule() override;
    void ShutdownModule() override;

  private:
};

FCommonGameModule::FCommonGameModule() {
}

void FCommonGameModule::StartupModule() {
}

void FCommonGameModule::ShutdownModule() {
}

IMPLEMENT_MODULE(FCommonGameModule, CommonGame);
