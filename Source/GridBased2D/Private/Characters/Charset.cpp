// "Unreal Pokémon" created by Retro & Chill.
#include "Characters/Charset.h"

UPaperFlipbook* UCharset::GetDownSprite() const {
	return DownSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetLeftSprite() const {
	return LeftSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetRightSprite() const {
	return RightSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetUpSprite() const {
	return UpSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetSprite(EFacingDirection Direction) const {
	auto SpriteData = GetDirectionalSprite(Direction);
	if (SpriteData == nullptr)
		return nullptr;

	return SpriteData->Flipbook;
}

bool UCharset::CanStopOnFrame(EFacingDirection Direction, int32 CurrentFrame) const {
	auto SpriteData = GetDirectionalSprite(Direction);
	if (SpriteData == nullptr)
		return false;

	return SpriteData->ValidStopFrames.Contains(CurrentFrame);
}

const FDirectionalSprite* UCharset::GetDirectionalSprite(EFacingDirection Direction) const {
	switch (Direction) {
		using enum EFacingDirection;
	case Down:
		return &DownSprite;
	case Left:
		return &LeftSprite;
	case Right:
		return &RightSprite;
	case Up:
		return &UpSprite;
	}

	return nullptr;
}
