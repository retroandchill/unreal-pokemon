// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SaveableEntity.generated.h"

struct FRPGEntitySaveDataHandle;

// This class does not need to be modified.
UINTERFACE()
class USaveableEntity : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class RPGSAVING_API ISaveableEntity
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Saveable")
    int32 GetVersion() const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "Saveable")
    void SaveCustomData(const FRPGEntitySaveDataHandle &Data) const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Saveable")
    void LoadCustomData(const FRPGEntitySaveDataHandle &Data);
};