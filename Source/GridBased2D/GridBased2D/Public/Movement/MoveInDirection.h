// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MoveInDirection.generated.h"

class IGridMovable;
class AGameCharacter;

/**
 * Delegate for handling the action when the player completes the action
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementFinished);

/**
 * Async action to move a character in a direction and wait until the movement has completed. When this action is going
 * on player control will be taken away if the character being moved is the player.
 */
UCLASS(meta = (HideThen))
class GRIDBASED2D_API UMoveInDirection : public UBlueprintAsyncActionBase {
	GENERATED_BODY()

public:
  /**
   * Called when the movement is finished
   */
  UPROPERTY(BlueprintAssignable)
  FOnMovementFinished OnMovementFinished;

 /**
  * Display a message to the player and call the following after the a choice as been selected
  * @param Character The character to move
  * @param MovementDirection The direction to move the character in
  * @return The node to execute the task with
  */
 UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Character|Movement")
 static UMoveInDirection* MoveInDirection(const TScriptInterface<IGridMovable> &Character, EFacingDirection MovementDirection);

 void Activate() override;

private:

 /**
  * The class used to display the message to the screen
  */
 UPROPERTY()
 TScriptInterface<IGridMovable> Character;

 /**
  * The direction to move the character in
  */
 EFacingDirection MovementDirection;
};
