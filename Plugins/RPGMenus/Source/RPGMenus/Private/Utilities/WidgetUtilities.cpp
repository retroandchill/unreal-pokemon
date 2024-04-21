// "Unreal Pokémon" created by Retro & Chill.
#include "Utilities/WidgetUtilities.h"
#include "Engine/UserInterfaceSettings.h"
#include "PaperSprite.h"

float UWidgetUtilities::GetWidgetDPIScale() {
    static constexpr float SlateDPI = 96.f;
#if WITH_EDITOR
    auto FontDPI = static_cast<float>(GetDefault<UUserInterfaceSettings>()->GetFontDisplayDPI());
    return SlateDPI / FontDPI;
#else
    return SlateDPI;
#endif
}

FVector2D UWidgetUtilities::GetDesiredBrushSize(const FSlateBrush &Brush) {
    auto ResourceObject = Brush.GetResourceObject();
    if (ResourceObject == nullptr)
        return Brush.ImageSize;

    if (auto Texture = Cast<UTexture2D>(ResourceObject); Texture != nullptr) {
        return GetDesiredTextureSize(Texture);
    }

    if (auto Sprite = Cast<UPaperSprite>(ResourceObject); Sprite != nullptr) {
#if WITH_EDITOR
        return Sprite->GetSourceSize();
#else
        return FVector2D(Sprite->GetBakedTexture()->GetSizeX(), Sprite->GetBakedTexture()->GetSizeY());
#endif
    }

    return Brush.ImageSize;
}

FVector2D UWidgetUtilities::GetDesiredTextureSize(const UTexture2D *const Texture) {
    if (Texture == nullptr)
        return FVector2D();

    return FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
}

void UWidgetUtilities::ChangeBrushSize(FSlateBrush &Brush, FVector2D NewSize) {
    Brush.SetImageSize(NewSize);
}
