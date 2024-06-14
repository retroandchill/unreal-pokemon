// "Unreal Pokémon" created by Retro & Chill.

#include "Primatives/NumberImageWidget.h"

UNumberImageWidget::UNumberImageWidget() {
    // We want an image for all 9 digits
    NumberImages.SetNum(10);
}

void UNumberImageWidget::SetNumber(uint32 NewNumber) {
    Number = NewNumber;
    RefreshNumber();   
}

TSharedRef<SWidget> UNumberImageWidget::RebuildWidget() {
    ImageBox = SNew(SHorizontalBox);

    for (auto AsString = FString::FromInt(Number); auto Digit : AsString) {
        int32 Index = TChar<TCHAR>::ConvertCharDigitToInt(Digit);
        auto &Brush = NumberImages[Index];
        auto Image = SNew(SImage).Image(&Brush);
        ImageBox->AddSlot().SizeParam(FAuto()).HAlign(HAlign_Left).VAlign(VAlign_Top)[Image];
        Images.Emplace(Image);
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

void UNumberImageWidget::RefreshNumber() {
    if (ImageBox == nullptr) {
        return;
    }

    auto AsString = FString::FromInt(Number);
    while (Images.Num() > AsString.Len()) {
        ImageBox->RemoveSlot(Images.Last().ToSharedRef());
        Images.Pop();
    }
    
    for (int32 i = 0; i < AsString.Len(); i++) {
        auto Digit = AsString[i];
        int32 Index = TChar<TCHAR>::ConvertCharDigitToInt(Digit);
        auto &Brush = NumberImages[Index];

        if (Images.IsValidIndex(i)) {
            Images[i]->SetImage(&Brush);
        } else {
            auto Image = SNew(SImage).Image(&Brush);
            ImageBox->AddSlot().SizeParam(FAuto()).HAlign(HAlign_Left).VAlign(VAlign_Top)[Image];
            Images.Emplace(Image);
        }
    }
}