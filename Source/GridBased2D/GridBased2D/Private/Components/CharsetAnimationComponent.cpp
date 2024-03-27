// "Unreal Pokémon" created by Retro & Chill.


#include "Components/CharsetAnimationComponent.h"

#include "Asserts.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"

void UCharsetAnimationComponent::BeginPlay() {
	Super::BeginPlay();
	auto Owner = GetOwner();
	GUARD(Owner == nullptr, )
	FlipbookComponent = Owner->FindComponentByClass<UPaperFlipbookComponent>();
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
}

void UCharsetAnimationComponent::UpdateDirection(EFacingDirection Direction) {
	CurrentDirection = Direction;
	ASSERT(FlipbookComponent != nullptr)
	auto Flipbook = GetDesiredFlipbook();
	FlipbookComponent->SetFlipbook(Flipbook);
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

UPaperFlipbook* UCharsetAnimationComponent::GetDesiredFlipbook() const {
	GUARD(Charset == nullptr, nullptr)
	return Charset->GetSprite(CurrentDirection);
}

