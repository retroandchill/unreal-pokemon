// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GridBasedCharacter.h"

#include "GridBasedBillboardCharacter.generated.h"

class UCapsuleComponent;
class UMaterialBillboardComponent;
class UCharacterBillboardAnimationComponent;

UCLASS()
class GRIDBASED2D_API AGridBasedBillboardCharacter : public AGridBasedCharacter {
    GENERATED_BODY()

  public:
    // Sets default values for this pawn's properties
    AGridBasedBillboardCharacter();

    void OnConstruction(const FTransform &Transform) override;

  private:
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
