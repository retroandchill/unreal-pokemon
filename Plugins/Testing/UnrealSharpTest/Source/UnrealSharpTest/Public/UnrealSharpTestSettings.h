// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "UnrealSharpTestSettings.generated.h"

/**
 * @class UUnrealSharpTestSettings
 * @brief A settings class for managing UnrealSharp configurations.
 *
 * This class extends from UDeveloperSettings and is designed to provide
 * configurable settings for UnrealSharp-related functionalities.
 * Settings declared in this class can be edited via the Unreal Editor or directly
 * in configuration files.
 *
 * It is placed under the "Engine" configuration category with a display name
 * of "UnrealSharp Test". The DefaultConfig specifier indicates that this class
 * will read and write its settings from and to the default configuration file.
 */
UCLASS(Config = Engine, meta = (DisplayName = "UnrealSharp Test"), DefaultConfig)
class UNREALSHARPTEST_API UUnrealSharpTestSettings : public UDeveloperSettings
{
    GENERATED_BODY()

  public:
    /**
     * @brief Retrieves the shared prefix string used in configurations or naming conventions.
     *
     * This method provides access to the shared prefix, which is a predefined and consistent
     * string used for identifying or organizing related elements in the system.
     * The prefix may be configured through settings and is read-only at runtime.
     *
     * @return A reference to the shared prefix string.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const FString &GetSharedPrefix() const
    {
        return SharedPrefix;
    }

  private:
    /**
     * @brief A predefined string prefix used for configuration or naming purposes.
     *
     * This property defines a shared prefix intended to provide consistency
     * in identifiers or other related elements within the system. It is configurable
     * through the editor and serves as a managed default string value that can be accessed
     * using the associated getter method, GetSharedPrefix.
     *
     * The property is marked as EditDefaultsOnly, meaning it can only be edited
     * within the editor's default settings and is not modifiable at runtime.
     * It belongs to the "Test Information" category for organizational purposes.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetSharedPrefix, Category = "Test Information")
    FString SharedPrefix = TEXT("Managed");
};
