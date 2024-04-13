// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/Window.h"
#include "Data/Windowskin.h"

UWindow::UWindow(const FObjectInitializer &ObjectInitializer) : UWidget(ObjectInitializer) {
    Brush.DrawAs = ESlateBrushDrawType::Box;
}

TSharedRef<SWidget> UWindow::RebuildWidget() {
    return SNew(SImage).Image(&Brush);
}

void UWindow::SynchronizeProperties() {
    Super::SynchronizeProperties();

    if (Windowskin != nullptr) {
        Brush.TintColor = FSlateColor(FColor(255, 255, 255));

        auto SourceTexture = Windowskin->GetSourceTexture();
        auto &Margins = Windowskin->GetMargins();
        double TextureWidth = SourceTexture->GetSizeX();
        double TextureHeight = SourceTexture->GetSizeY();

        Brush.SetResourceObject(SourceTexture);
        Brush.Margin = FMargin(Margins.Left / TextureWidth, Margins.Top / TextureHeight, Margins.Right / TextureWidth,
                               Margins.Bottom / TextureHeight);
    } else {
        Brush.TintColor = FSlateColor(FColor(0, 0, 0, 0));
        Brush.SetResourceObject(nullptr);
    }
}
