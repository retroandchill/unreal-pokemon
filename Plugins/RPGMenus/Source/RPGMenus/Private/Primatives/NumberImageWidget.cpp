// "Unreal Pokémon" created by Retro & Chill.

#include "Primatives/NumberImageWidget.h"

UNumberImageWidget::UNumberImageWidget() {
    // We want an image for all 9 digits
    NumberImages.SetNum(10);
}

void UNumberImageWidget::SetNumber(uint32 NewNumber) {
    Number = NewNumber;
    RebuildWidget();
}

TSharedRef<SWidget> UNumberImageWidget::RebuildWidget() {
    ImageBox = SNew(SHorizontalBox);

    for (auto AsString = FString::FromInt(Number); auto Digit : AsString) {
        int32 Index = TChar<TCHAR>::ConvertCharDigitToInt(Digit);
        auto &Brush = NumberImages[Index];
        ImageBox->AddSlot().SizeParam(FAuto()).HAlign(HAlign_Left).VAlign(VAlign_Top)[SNew(SImage).Image(&Brush)];
    }

    return ImageBox.ToSharedRef();
}

void UNumberImageWidget::ReleaseSlateResources(bool bReleaseChildren) {
    Super::ReleaseSlateResources(bReleaseChildren);
    ImageBox.Reset();
}

void UNumberImageWidget::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    RebuildWidget();
}

void UNumberImageWidget::PostEditChangeChainProperty(FPropertyChangedChainEvent &PropertyChangedEvent) {
    Super::PostEditChangeChainProperty(PropertyChangedEvent);
    RebuildWidget();
}