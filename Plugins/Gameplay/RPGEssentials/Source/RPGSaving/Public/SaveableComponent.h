// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RPGEntitySaveData.h"
#include "UObject/Interface.h"

#include "SaveableComponent.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USaveableComponent : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class RPGSAVING_API ISaveableComponent
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Saveable")
    int32 GetVersion() const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "Saveable")
    FRPGComponentSaveDataHandle CreateSaveData() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Saveable")
    void LoadSaveData(const FRPGComponentSaveDataHandle &SaveData);
};