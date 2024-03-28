// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GridBasedPawn.h"
#include "GridBasedBillboardPawn.generated.h"


class UCapsuleComponent;
class UMaterialBillboardComponent;
class UCharacterBillboardAnimationComponent;

UCLASS()
class GRIDBASED2D_API AGridBasedBillboardPawn : public AGridBasedPawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGridBasedBillboardPawn();

	void OnConstruction(const FTransform& Transform) override;

private:
	/**
	 * The CapsuleComponent used for collision checks within the grid.
	 */
	UPROPERTY(VisibleAnywhere, Category = Components, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	/**
	 * The component used for billboard animations
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterBillboardAnimationComponent> BillboardAnimationComponent;

	/**
	 * The component used for actually displaying the Billboard
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialBillboardComponent> MaterialBillboardComponent;
};
