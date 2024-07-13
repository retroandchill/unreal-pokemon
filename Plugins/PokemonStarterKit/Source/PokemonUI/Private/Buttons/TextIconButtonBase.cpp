// "Unreal Pokémon" created by Retro & Chill.


#include "Buttons/TextIconButtonBase.h"
#include "Components/Image.h"

void UTextIconButtonBase::NativePreConstruct() {
    Super::NativePreConstruct();
    if (Icon != nullptr) {
        Icon->SetBrush(IconImage);
    }
}

const FSlateBrush & UTextIconButtonBase::GetIconImage() const {
    return IconImage;
}

void UTextIconButtonBase::SetIconImage(const FSlateBrush &NewImage) {
    IconImage = NewImage;
    if (Icon != nullptr && !IsHovered()) {
        Icon->SetBrush(IconImage);
    }
}

const FSlateBrush & UTextIconButtonBase::GetIconImageHovered() const {
    return IconImageHovered;
}

void UTextIconButtonBase::SetIconImageHovered(const FSlateBrush &NewImage) {
    IconImageHovered = NewImage;
    if (Icon != nullptr && IsHovered()) {
        Icon->SetBrush(IconImageHovered);
    }
}

UImage * UTextIconButtonBase::GetIcon() const {
    return Icon;
}

void UTextIconButtonBase::NativeOnHovered() {
    Super::NativeOnHovered();
    Icon->SetBrush(IconImageHovered);
}

void UTextIconButtonBase::NativeOnUnhovered() {
    Super::NativeOnUnhovered();
    Icon->SetBrush(IconImage);
}