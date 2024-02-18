// Created by Retro & Chill.


#include "Windows/Windowskin.h"

UTexture2D* UWindowskin::GetSourceTexture() const {
	return SourceTexture;
}

const FMargin& UWindowskin::GetMargins() const {
	return Margins;
}
