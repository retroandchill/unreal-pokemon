// "Unreal Pokémon" created by Retro & Chill.


#include "Components/CharsetAnimationComponent.h"

#include "Asserts.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"
#include "Components/GridBasedMovement.h"

void UCharsetAnimationComponent::UpdateDirection(EFacingDirection Direction) {
	CurrentDirection = Direction;
	auto Flipbook = GetDesiredFlipbook();
	GUARD(FlipbookComponent == nullptr, )
	FlipbookComponent->SetFlipbook(Flipbook);
}

bool UCharsetAnimationComponent::IsMoveAnimationPlaying() const {
	ASSERT(FlipbookComponent != nullptr)
	return FlipbookComponent->IsPlaying();
}

void UCharsetAnimationComponent::StartMoveAnimation() {
	ASSERT(FlipbookComponent != nullptr)
	FlipbookComponent->PlayFromStart();
	FlipbookComponent->SetLooping(true);
}

bool UCharsetAnimationComponent::CanStopMoving() const {
	ASSERT(FlipbookComponent != nullptr)
	return Charset == nullptr || Charset->CanStopOnFrame(CurrentDirection, FlipbookComponent->GetPlaybackPositionInFrames());
}

void UCharsetAnimationComponent::StopMoveAnimation() {
	ASSERT(FlipbookComponent != nullptr)
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
}

UPaperFlipbookComponent* UCharsetAnimationComponent::GetFlipbookComponent() const {
	return FlipbookComponent;
}

void UCharsetAnimationComponent::SetFlipbookComponent(UPaperFlipbookComponent* NewFlipbookComponent) {
	FlipbookComponent = NewFlipbookComponent;

#if WITH_EDITOR
	// In the editor we want to set the position of the character in question
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
#endif
}

TScriptInterface<IGridBasedMovement> UCharsetAnimationComponent::GetGridBasedMovement() const {
	return GridBasedMovement;
}

void UCharsetAnimationComponent::SetGridBasedMovement(TScriptInterface<IGridBasedMovement> NewGridBasedMovement) {
	GridBasedMovement = NewGridBasedMovement;
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

UPaperFlipbook* UCharsetAnimationComponent::GetDesiredFlipbook() const {
	GUARD(Charset == nullptr, nullptr)
	return Charset->GetSprite(CurrentDirection);
}

