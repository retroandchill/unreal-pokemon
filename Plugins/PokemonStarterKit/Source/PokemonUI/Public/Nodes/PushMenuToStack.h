// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Engine/CancellableAsyncAction.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PushMenuToStack.generated.h"

class UPokemonMenuBase;

namespace Pokemon::UI {

/**
 * The native tag assigned the primary layer for display
 */
POKEMONUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PokemonMenuLayerTag)

}

/**
 * Delegate used for when the menu is loaded in for the game
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuLoaded, UPokemonMenuBase*, Menu);

/**
 * Push the specified menu onto the stack for display.
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UPushMenuToStack : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    /**
     * Display a message to the player and call the following after the message is done
     * @param WorldContextObject The object used to obtain the state of the world
     * @param MenuClass The type of screen to be loaded
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = UI, meta = (WorldContext = "WorldContextObject"))
    static UPushMenuToStack *PushMenuToStack(const UObject *WorldContextObject, TSoftClassPtr<UPokemonMenuBase> MenuClass);

    void Activate() override;
    void SetReadyToDestroy() override;

private:
    /**
     * Called when the menu is loaded
     */
    UPROPERTY(BlueprintAssignable)
    FOnMenuLoaded MenuLoaded;

    /**
     * The object used to obtain the state of the world
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContextObject;
    
    /**
     * The type of screen to be loaded
     */
    UPROPERTY()
    TSoftClassPtr<UPokemonMenuBase> MenuClass;

    TSharedPtr<FStreamableHandle> LoadHandle;

};
