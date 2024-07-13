// "Unreal Pokémon" created by Retro & Chill.


#include "Buttons/PokemonTextButtonBase.h"
#include "CommonTextBlock.h"

void UPokemonTextButtonBase::NativePreConstruct() {
    Super::NativePreConstruct();
    if (TextBlock != nullptr) {
        TextBlock->SetText(Text);
    }    
}

const FText & UPokemonTextButtonBase::GetText() const {
    return Text;
}

void UPokemonTextButtonBase::SetText(const FText &NewText) {
    Text = NewText;
    if (TextBlock != nullptr) {
        TextBlock->SetText(NewText);
    }
}

UCommonTextBlock * UPokemonTextButtonBase::GetTextBlock() const {
    return TextBlock;
}

void UPokemonTextButtonBase::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();
    if (TextBlock != nullptr) {
        TextBlock->SetStyle(GetCurrentTextStyleClass());
    }
}