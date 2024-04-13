// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "TestShutdownSubsystem.generated.h"

/**
 * @class UTestShutdownSubsystem
 *
 * A class representing the shutdown subsystem for the automation testing. It is activated by invoking the "ShutdownAfterTests"
 * console command after after which it will wait until the automation system has concluded and then perform a full engine shutdown.
 */
UCLASS()
class POKEMONTESTS_API UTestShutdownSubsystem : public UEditorSubsystem, public FTickableEditorObject {
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;

    /**
     * @brief GetInstance
     *
     * Returns the instance of the UTestShutdownSubsystem class.
     *
     * @return UTestShutdownSubsystem& - A reference to the UTestShutdownSubsystem instance.
     */
    static UTestShutdownSubsystem &GetInstance();

    TStatId GetStatId() const override;
    bool IsTickable() const override;
    void Tick(float DeltaTime) override;

    /**
     * @brief RequestExitAfterTestsComplete
     *
     * A method in the UTestShutdownSubsystem class that sets the bExitRequested flag to true.
     * This method is called to request an engine shutdown after the completion of automation tests.
     *
     * @details This method is called to request an engine shutdown after the completion of automation tests.
     * It sets the bExitRequested flag to true, indicating that an engine shutdown has been requested.
     * The engine will exit once the automation system has concluded.
     */
    UFUNCTION(BlueprintCallable, Category = "Shutdown")
    void RequestExitAfterTestsComplete();

private:
    /**
     * A pointer to the instance of the UTestShutdownSubsystem class. It is initially set to nullptr.
     * An instance of the UTestShutdownSubsystem class can be obtained by calling the GetInstance() method.
     */
    static UTestShutdownSubsystem* Instance;

    /**
     * A boolean variable indicating whether the automation testing has started or not.
     */
    bool bAutomationStarted = false;

    /**
     * A boolean variable indicating whether an engine shutdown has been requested or not.
     */
    bool bExitRequested = false;
};
