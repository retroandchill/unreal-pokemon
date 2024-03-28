// "Unreal Pokémon" created by Retro & Chill.


#include "Components/CharacterBillboardAnimationComponent.h"

#include "Asserts.h"
#include "Components/MaterialBillboardComponent.h"


void UCharacterBillboardAnimationComponent::UpdateDirection(EFacingDirection Direction) {
	if (MaterialInstance == nullptr) {
		SetUpMaterialInstance();
	}
	MaterialInstance->SetScalarParameterValue(DirectionPropertyName, static_cast<float>(Direction));
}

bool UCharacterBillboardAnimationComponent::IsMoveAnimationPlaying() const {
	return MaterialInstance != nullptr && bIsPlaying;
}

void UCharacterBillboardAnimationComponent::StartMoveAnimation() {
	if (MaterialInstance == nullptr) {
		SetUpMaterialInstance();
	}
	MaterialInstance->SetScalarParameterValue(FrameRatePropertyName, OriginalFrameRate);
}

bool UCharacterBillboardAnimationComponent::CanStopMoving() const {
	return true;
}

void UCharacterBillboardAnimationComponent::StopMoveAnimation() {
	if (MaterialInstance == nullptr) {
		SetUpMaterialInstance();
	}
	MaterialInstance->SetScalarParameterValue(FrameRatePropertyName, 0);
}

void UCharacterBillboardAnimationComponent::BeginPlay() {
	Super::BeginPlay();

	SetUpMaterialInstance();
	GUARD(BillboardComponent == nullptr, )
	BillboardComponent->Elements.Empty();
	BillboardComponent->AddElement(MaterialInstance, nullptr, false, SourceTexture->GetSizeY() / (4 * 2), SourceTexture->GetSizeX() / (4 * 2), nullptr);
}

void UCharacterBillboardAnimationComponent::SetUpMaterialInstance() {
	GUARD(BaseMaterial == nullptr || SourceTexture == nullptr, );
	MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	OriginalFrameRate = MaterialInstance->K2_GetScalarParameterValue(FrameRatePropertyName);
	MaterialInstance->SetTextureParameterValue(SourceTexturePropertyName, SourceTexture);
	MaterialInstance->SetScalarParameterValue(FrameRatePropertyName, 0);

	GUARD(BillboardComponent == nullptr, )
	BillboardComponent->Elements.Empty();
	BillboardComponent->AddElement(MaterialInstance, nullptr, false, SourceTexture->GetSizeY() / (4 * 2), SourceTexture->GetSizeX() / (4 * 2), nullptr);
}

UMaterialBillboardComponent* UCharacterBillboardAnimationComponent::GetBillboardComponent() const {
	return BillboardComponent;
}

void UCharacterBillboardAnimationComponent::SetBillboardComponent(UMaterialBillboardComponent* NewBillboardComponent) {
	BillboardComponent = NewBillboardComponent;
}

UMaterialInstanceDynamic* UCharacterBillboardAnimationComponent::GetMaterialInstance() const {
	return MaterialInstance;
}

void UCharacterBillboardAnimationComponent::SetMaterialInstance(UMaterialInstanceDynamic* NewMaterialInstance) {
	MaterialInstance = NewMaterialInstance;
}
