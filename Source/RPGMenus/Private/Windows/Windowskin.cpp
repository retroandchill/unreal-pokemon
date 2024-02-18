// Created by Retro & Chill.


#include "Windows/Windowskin.h"

UTexture2D* UWindowskin::GetSourceTexture() const {
	return SourceTexture;
}

const FBox2D &UWindowskin::GetMargins() const {
	return Margins;
}
