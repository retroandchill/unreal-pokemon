//==========================================================================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "PaperFlipbookWidgetEditorStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "SlateOptMacros.h"	
#include "Styling/SlateTypes.h"
#include "Styling/SlateStyle.h"





#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FPaperFlipbookWidgetEditorStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )


FString FPaperFlipbookWidgetEditorStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString IconsDir = IPluginManager::Get().FindPlugin(TEXT("PaperFlipbookWidget"))->GetContentDir() / TEXT("Editor");
	return (IconsDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FPaperFlipbookWidgetEditorStyle::StyleSet = NULL;
TSharedPtr< class ISlateStyle > FPaperFlipbookWidgetEditorStyle::Get() { return StyleSet; }


FName FPaperFlipbookWidgetEditorStyle::GetAppStyleSetName()
{
	return StyleSet->GetStyleSetName();
}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FPaperFlipbookWidgetEditorStyle::Initialize()
{

	// Const icon & thumbnail sizes
	const FVector2D Icon16x16(16.0f, 16.f);
	const FVector2D Icon64x64(64.f, 64.f);


	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}

	// Style Set Allocation
	StyleSet = MakeShareable(new FSlateStyleSet("PaperFlipbookWidgetStyle"));
	FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("PaperFlipbookWidget"))->GetContentDir();
	StyleSet->SetContentRoot(ContentDir);

	
	// Class Thumbnails
	StyleSet->Set("ClassIcon.PaperFlipbookUserWidget", new IMAGE_BRUSH("PaperFlipbookUserWidget_64x", Icon16x16));
	StyleSet->Set("ClassThumbnail.PaperFlipbookUserWidget", new IMAGE_BRUSH("PaperFlipbookUserWidget_64x", Icon64x64));


	// Register Style Set
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef IMAGE_BRUSH


void FPaperFlipbookWidgetEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

