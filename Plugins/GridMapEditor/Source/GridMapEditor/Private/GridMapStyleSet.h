#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FGridMapStyleSet : public FSlateStyleSet
{
	typedef FSlateStyleSet Super;
	
	static const FVector2D Icon14x14;
	static const FVector2D Icon16x16;
	static const FVector2D Icon20x20;
	static const FVector2D Icon40x40;
	static const FVector2D Icon64x64;
	static const FVector2D Icon512x512;

public:
	static const FMargin StandardPadding;
	static const FMargin StandardLeftPadding;
	static const FMargin StandardRightPadding;

	static const FSlateFontInfo StandardFont;

public:
	static const FName Name;

public:
	FGridMapStyleSet(const FString& PluginDir);

private:
	const FString ContentGridMapDir;

};