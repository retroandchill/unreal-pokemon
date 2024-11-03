// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveableSubsystem.generated.h"

class ISerializable;

// This class does not need to be modified.
UINTERFACE()
class USaveableSubsystem : public UInterface {
    GENERATED_BODY()
};

/**
 * A type of subsystem that can send data for saving.
 */
class ENHANCEDSAVEGAME_API ISaveableSubsystem {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent, Category = Saving)
    TScriptInterface<ISerializable> CreateSaveData() const;

    UFUNCTION(BlueprintNativeEvent, Category = Saving)
    void LoadSaveData(const TScriptInterface<ISerializable>& SaveData);

};