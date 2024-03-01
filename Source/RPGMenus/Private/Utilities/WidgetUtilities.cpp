// "Unreal Pokémon" created by Retro & Chill.
#include "Utilities/WidgetUtilities.h"

#include "Engine/UserInterfaceSettings.h"
#include "PaperSprite.h"

float UWidgetUtilities::GetWidgetDPIScale() {
	static constexpr float SlateDPI = 96.f;
	auto FontDPI = static_cast<float>(GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetFontDisplayDPI());
	return SlateDPI / FontDPI;
}

FVector2D UWidgetUtilities::GetDesiredBrushSize(const FSlateBrush& Brush) {
	auto ResourceObject = Brush.GetResourceObject();
	if (ResourceObject == nullptr)
		return Brush.ImageSize;

	if (auto Texture = Cast<UTexture2D>(ResourceObject); Texture != nullptr) {
		return GetDesiredTextureSize(Texture);
	}

	if (auto Sprite = Cast<UPaperSprite>(ResourceObject); Sprite != nullptr) {
		return Sprite->GetSourceSize();
	}
	
	return Brush.ImageSize;
}

FVector2D UWidgetUtilities::GetDesiredTextureSize(UTexture2D* Texture) {
	if (Texture == nullptr)
		return FVector2D();
	
	return FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
}

void UWidgetUtilities::ChangeBrushSize(FSlateBrush& Brush, FVector2D NewSize) {
	Brush.SetImageSize(NewSize);
}
