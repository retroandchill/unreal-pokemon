// "Unreal Pokémon" created by Retro & Chill.


#include "Characters/GridBasedBillboardPawn.h"

#include "GridUtils.h"
#include "PaperTileMapComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterBillboardAnimationComponent.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/MaterialBillboardComponent.h"


// Sets default values
AGridBasedBillboardPawn::AGridBasedBillboardPawn() {
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	float GridSize = static_cast<float>(UGridUtils::GetGridSize());
	CapsuleComponent->InitCapsuleSize(GridSize / 2, GridSize / 2);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	MaterialBillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("Sprite"));
	MaterialBillboardComponent->SetupAttachment(CapsuleComponent);

	BillboardAnimationComponent = CreateDefaultSubobject<UCharacterBillboardAnimationComponent>(TEXT("CharacterAnimation"));
	
}

void AGridBasedBillboardPawn::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	GetGridBasedMovementComponentInternal()->SetGridBasedAnimationComponent(BillboardAnimationComponent);
	BillboardAnimationComponent->SetBillboardComponent(MaterialBillboardComponent);
	BillboardAnimationComponent->SetUpMaterialInstance();
}

