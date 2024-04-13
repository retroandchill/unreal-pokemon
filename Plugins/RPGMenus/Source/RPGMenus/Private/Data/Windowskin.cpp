// "Unreal Pokémon" created by Retro & Chill.
#include "Data/Windowskin.h"

UTexture2D *UWindowskin::GetSourceTexture() const {
    return SourceTexture;
}

const FMargin &UWindowskin::GetMargins() const {
    return Margins;
}
