// "Unreal Pokémon" created by Retro & Chill.


#include "Components/CharsetAnimationComponent.h"

#include "Asserts.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"

void UCharsetAnimationComponent::UpdateDirection(EFacingDirection Direction) {
	CurrentDirection = Direction;
	auto Flipbook = GetDesiredFlipbook();
	GetFlipbookComponent()->SetFlipbook(Flipbook);
}

bool UCharsetAnimationComponent::IsMoveAnimationPlaying() const {
	ASSERT(FlipbookComponent != nullptr)
	return FlipbookComponent->IsPlaying();
}

void UCharsetAnimationComponent::StartMoveAnimation() {
	FlipbookComponent->PlayFromStart();
	FlipbookComponent->SetLooping(true);
}

bool UCharsetAnimationComponent::CanStopMoving() {
	ASSERT(FlipbookComponent != nullptr)
	return Charset == nullptr || Charset->CanStopOnFrame(CurrentDirection, FlipbookComponent->GetPlaybackPositionInFrames());
}

void UCharsetAnimationComponent::StopMoveAnimation() {
	ASSERT(FlipbookComponent != nullptr)
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
}

UPaperFlipbookComponent* UCharsetAnimationComponent::GetFlipbookComponent() {
	if (FlipbookComponent != nullptr) {
		return FlipbookComponent;
	}

	auto Owner = GetOwner();
	GUARD(Owner == nullptr, nullptr)
	FlipbookComponent = Owner->FindComponentByClass<UPaperFlipbookComponent>();
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
	return FlipbookComponent;
}

UCharset* UCharsetAnimationComponent::GetCharset() const {
	return Charset;
}

void UCharsetAnimationComponent::SetCharset(UCharset* NewCharset) {
	Charset = NewCharset;
	UpdateDirection(CurrentDirection);
}

UPaperFlipbook* UCharsetAnimationComponent::GetDesiredFlipbook() const {
	GUARD(Charset == nullptr, nullptr)
	return Charset->GetSprite(CurrentDirection);
}

