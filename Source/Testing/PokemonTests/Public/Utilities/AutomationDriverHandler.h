// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "AutomationDriverTypeDefs.h"
#include "CoreMinimal.h"

/**
 * Handler class to work with setting up the automation driver
 */
class POKEMONTESTS_API FAutomationDriverHandler {
  public:
    FAutomationDriverHandler();
    FAutomationDriverHandler(const FAutomationDriverHandler &Other) = delete;
    FAutomationDriverHandler(FAutomationDriverHandler &&Other) noexcept = delete;
    ~FAutomationDriverHandler();

    FAutomationDriverHandler &operator=(const FAutomationDriverHandler &Other) = delete;
    FAutomationDriverHandler &operator=(FAutomationDriverHandler &&Other) = delete;

    FAutomationDriverPtr CreateDriver() const;
};
