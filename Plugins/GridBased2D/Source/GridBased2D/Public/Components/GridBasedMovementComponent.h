// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"
#include "Characters/MoveCheckResult.h"
#include "Components/ActorComponent.h"
#include "Engine/OverlapResult.h"
#include "GridBasedMovement.h"
#include "Map/WithinMap.h"

#include "GridBasedMovementComponent.generated.h"

class UCharacterMovementComponent;
class IMapGrid;
class IInteractable;
class AGridBasedMap;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDirectionChange, EFacingDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterMovementStateChange, bool, bIsMoving);

/**
 * Component used to handle the movement of an actor along a fixed size grid.
 */
UCLASS(ClassGroup = (Movement), meta = (BlueprintSpawnableComponent))
class GRIDBASED2D_API UGridBasedMovementComponent : public UActorComponent,
                                                    public IWithinMap,
                                                    public IGridBasedMovement {
    GENERATED_BODY()

  public:
    /**
     * Construct the default object
     */
    UGridBasedMovementComponent();

  protected:
    void BeginPlay() override;

  public:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    /**
     * Move the character in the specified direction
     * @param MovementDirection The direction to move the character in
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Movement")
    void MoveInDirection(EFacingDirection MovementDirection);

    /**
     * Move the character in the specified direction
     * @param MovementDirection The direction to move the character in
     * @param MovementCompleteCallback The functor to call when movement is completed
     */
    void MoveInDirection(EFacingDirection MovementDirection, TFunction<void()> &&MovementCompleteCallback);

    /**
     * Check to see if the character can move in the specified direction
     * @param MovementDirection The direction the character would like to move in
     * @return Can the character move to that tile, as well and any interactable objects found when the check is done
     */
    UFUNCTION(BlueprintPure, Category = "Character|Movement")
    FMoveCheckResult MovementCheck(EFacingDirection MovementDirection) const;

    /**
     * Can this character move between the various maps of the overworld
     * @return Can the character move between maps?
     */
    bool CanMoveBetweenMaps() const;

    /**
     * Turn the character to face a specific direction
     * @param FacingDirection The direction to update the character's facing direction to
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Movement")
    void FaceDirection(EFacingDirection FacingDirection);

    /**
     * Warp to the given location and offset
     * @param X The X coordinate to warp to
     * @param Y The Y coordinate to warp to
     * @param Offset The physical offset of the map in the world
     */
    UFUNCTION(BlueprintCallable, Category = "Character|Movement")
    void WarpToLocation(int32 X, int32 Y, FVector Offset) override;

    /**
     * Called when the player changes maps
     * @param NewMap The new map the player changes to
     */
    void OnMapChanged(IMapGrid &NewMap) const;

    FIntVector2 GetCurrentPosition() const override;

    /**
     * Get the position that this character is currently moving to
     * @return The intended grid position of the character
     */
    FIntVector2 GetDesiredPosition() const;

    UFUNCTION(BlueprintPure, Category = "Character|Movement")
    EFacingDirection GetDirection() const final;

    UFUNCTION(BlueprintCallable, Category = "Character|Movement")
    bool IsMoving() const override;

    UFUNCTION(BlueprintCallable, Category = "Character|Movement", meta = (AutoCreateRefTerm = MovementVector))
    void MoveInput(const FVector2D& InputVector) override;

    UFUNCTION(BlueprintCallable, Category = "Character|Movement", meta = (AutoCreateRefTerm = MovementVector))
    void TurnInput(const FVector2D& InputVector) override;

    /**
     * Perform a hit test on the tile in the given direction.
     * @param MovementDirection The direction to check the file of
     * @return Any hits that are found and all interactable actors on the tile
     */
    TArray<FOverlapResult> HitTestOnFacingTile(EFacingDirection MovementDirection) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Character|Movement", meta = (AutoCreateRefTerm = MovementVector))
    TArray<TScriptInterface<IInteractable>> InteractTestOnFacingTile(EFacingDirection MovementDirection) const;

    /**
     * Perform a hit interaction on all of the interactable objects in front of the player
     * @param Interactables The interactable objects in question
     */
    void HitInteraction(const TArray<TScriptInterface<IInteractable>> &Interactables) const;

  private:
    /**
     * Update the character's movement upon a tick
     * @param DeltaTime The amount of time that has passed since the previous frame
     */
    void UpdateMovement(float DeltaTime);

    /**
     * Update the character's animation state
     * @param DeltaTime The amount of time that has passed since the previous frame
     */
    void UpdateAnimation(float DeltaTime);

    /**
     * Called when the player completes their movement cycle
     */
    void MoveComplete();

    /**
     * The current position of the character within the movement grid
     */
    UPROPERTY(VisibleAnywhere, Category = "Position")
    FIntVector2 CurrentPosition;

    /**
     * The desired position of the character within the movement grid
     */
    UPROPERTY(VisibleAnywhere, Category = "Position")
    FIntVector2 DesiredPosition;

    /**
     * The direction this character is facing
     */
    UPROPERTY(EditAnywhere, Category = "Character")
    EFacingDirection Direction = EFacingDirection::Down;

    /**
     * Called when the character changes direction
     */
    UPROPERTY(BlueprintAssignable)
    FOnCharacterDirectionChange OnDirectionChange;

    UPROPERTY(BlueprintAssignable)
    FOnCharacterMovementStateChange OnMovementStateChange;

    /**
     * The timer for movement used to linearly interpolate the position to the new one
     */
    TOptional<float> MoveTimer;

    /**
     * One time callback for when movement is complete.
     */
    TOptional<TFunction<void()>> MoveCallback;

    /**
     * The timer for movement used to determine where to stop animation
     */
    TOptional<float> StopTimer;

    /**
     * Should the character perform a sweep when setting the actor to a location?
     */
    bool bPerformSweep = false;

    bool bIsMoving = false;
};
