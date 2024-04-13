// "Unreal Pokémon" created by Retro & Chill.

#include "Characters/GridBasedBillboardCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterBillboardAnimationComponent.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/MaterialBillboardComponent.h"

// Sets default values
AGridBasedBillboardCharacter::AGridBasedBillboardCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    MaterialBillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Sprite"));
    MaterialBillboardComponent->SetupAttachment(GetCapsuleComponent());

    BillboardAnimationComponent =
        CreateDefaultSubobject<UCharacterBillboardAnimationComponent>(TEXT("CharacterAnimation"));
}

void AGridBasedBillboardCharacter::OnConstruction(const FTransform &Transform) {
    Super::OnConstruction(Transform);
    GetGridBasedMovementComponentInternal()->SetGridBasedAnimationComponent(BillboardAnimationComponent);
    BillboardAnimationComponent->SetBillboardComponent(MaterialBillboardComponent);
    BillboardAnimationComponent->SetUpMaterialInstance();
}
