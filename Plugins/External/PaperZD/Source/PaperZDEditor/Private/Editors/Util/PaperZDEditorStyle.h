// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Styling/ISlateStyle.h"

/**
 * Style class for PaperZD
 */
class FPaperZDEditorStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static const ISlateStyle& Get();
	static FName GetStyleSetName();	

	/* Helper for getting the brush right away. */
	static const FSlateBrush* GetBrush(FName PropertyName, const ANSICHAR* Specifier = NULL);

private:
	static TSharedRef<class FSlateStyleSet> Create();

private:
	static TSharedPtr<class FSlateStyleSet> Instance;
};
