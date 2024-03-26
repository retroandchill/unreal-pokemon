// "Unreal Pokémon" created by Retro & Chill.


#include "Components/CharsetComponent.h"

#include "Asserts.h"
#include "PaperFlipbookComponent.h"
#include "Characters/Charset.h"

void UCharsetComponent::BeginPlay() {
	Super::BeginPlay();
	auto Owner = GetOwner();
	GUARD(Owner == nullptr, )
	FlipbookComponent = Owner->FindComponentByClass<UPaperFlipbookComponent>();
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
}

void UCharsetComponent::UpdateDirection(EFacingDirection Direction) {
	CurrentDirection = Direction;
	ASSERT(FlipbookComponent != nullptr)
	auto Flipbook = GetDesiredFlipbook();
	FlipbookComponent->SetFlipbook(Flipbook);
}

bool UCharsetComponent::IsMoveAnimationPlaying() const {
	ASSERT(FlipbookComponent != nullptr)
	return FlipbookComponent->IsPlaying();
}

void UCharsetComponent::StartMoveAnimation() {
	FlipbookComponent->PlayFromStart();
	FlipbookComponent->SetLooping(true);
}

bool UCharsetComponent::CanStopMoving() {
	ASSERT(FlipbookComponent != nullptr)
	return Charset == nullptr || Charset->CanStopOnFrame(CurrentDirection, FlipbookComponent->GetPlaybackPositionInFrames());
}

void UCharsetComponent::StopMoveAnimation() {
	ASSERT(FlipbookComponent != nullptr)
	FlipbookComponent->Stop();
	FlipbookComponent->SetPlaybackPositionInFrames(0, false);
}

UPaperFlipbook* UCharsetComponent::GetDesiredFlipbook() const {
	GUARD(Charset == nullptr, nullptr)
	return Charset->GetSprite(CurrentDirection);
}

