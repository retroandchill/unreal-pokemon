// "Unreal Pokémon" created by Retro & Chill.


#include "Components/CharsetAnimationComponent.h"

#include "Asserts.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"
#include "Components/GridBasedMovement.h"

void UCharsetAnimationComponent::UpdateDirection(EFacingDirection Direction) {
	CurrentDirection = Direction;
	auto Flipbook = GetDesiredFlipbook();
	auto AnimComp = GetFlipbookComponent();
	GUARD(AnimComp == nullptr, )
	AnimComp->SetFlipbook(Flipbook);
}

bool UCharsetAnimationComponent::IsMoveAnimationPlaying() {
	auto AnimComp = GetFlipbookComponent();
	ASSERT(AnimComp != nullptr)
	return FlipbookComponent->IsPlaying();
}

void UCharsetAnimationComponent::StartMoveAnimation() {
	auto AnimComp = GetFlipbookComponent();
	ASSERT(AnimComp != nullptr)
	AnimComp->PlayFromStart();
	AnimComp->SetLooping(true);
}

bool UCharsetAnimationComponent::CanStopMoving() {
	auto AnimComp = GetFlipbookComponent();
	ASSERT(AnimComp != nullptr)
	return Charset == nullptr || Charset->CanStopOnFrame(CurrentDirection, AnimComp->GetPlaybackPositionInFrames());
}

void UCharsetAnimationComponent::StopMoveAnimation() {
	auto AnimComp = GetFlipbookComponent();
	ASSERT(AnimComp != nullptr)
	AnimComp->Stop();
	AnimComp->SetPlaybackPositionInFrames(0, false);
}

UPaperFlipbookComponent* UCharsetAnimationComponent::GetFlipbookComponent() {
	if (FlipbookComponent != nullptr) {
		return FlipbookComponent;
	}

	auto Owner = GetOwner();
	GUARD(Owner == nullptr, nullptr)
	FlipbookComponent = Owner->FindComponentByClass<UPaperFlipbookComponent>();

#if WITH_EDITOR
	// In the editor we want to set the position of the character in question
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
#endif
	
	return FlipbookComponent;
}

UCharset* UCharsetAnimationComponent::GetCharset() const {
	return Charset;
}

void UCharsetAnimationComponent::SetCharset(UCharset* NewCharset) {
	Charset = NewCharset;

	if (auto MovementComponent = GetGridBasedMovement(); MovementComponent != nullptr) {
		CurrentDirection = MovementComponent->GetDirection();
	}
	
	UpdateDirection(CurrentDirection);
#if WITH_EDITOR
	// In the editor we want to set character as not animating
	GUARD(FlipbookComponent == nullptr, )
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
#endif
}

TScriptInterface<IGridBasedMovement> UCharsetAnimationComponent::GetGridBasedMovement() {
	if (GridBasedMovement != nullptr) {
		return GridBasedMovement;
	}

	auto Owner = GetOwner();
	GUARD(Owner == nullptr, nullptr)
	GridBasedMovement = Owner->FindComponentByInterface(UGridBasedMovement::StaticClass());
	return GridBasedMovement;
}

UPaperFlipbook* UCharsetAnimationComponent::GetDesiredFlipbook() const {
	GUARD(Charset == nullptr, nullptr)
	return Charset->GetSprite(CurrentDirection);
}

