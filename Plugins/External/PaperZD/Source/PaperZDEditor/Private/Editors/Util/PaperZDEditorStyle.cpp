// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDEditorStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "EditorStyleSet.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr<FSlateStyleSet> FPaperZDEditorStyle::Instance = nullptr;

void FPaperZDEditorStyle::Initialize()
{
	if (!Instance.IsValid())
	{
		Instance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*Instance);
	}
}

void FPaperZDEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*Instance);
	ensure(Instance.IsUnique());
	Instance.Reset();
}

FName FPaperZDEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("PaperZDEditorStyle"));
	return StyleSetName;
}

const ISlateStyle& FPaperZDEditorStyle::Get()
{
	return *Instance;
}

const FSlateBrush* FPaperZDEditorStyle::GetBrush(FName PropertyName, const ANSICHAR* Specifier /* = NULL */)
{
	return Instance->GetBrush(PropertyName, Specifier);
}

#define IMAGE_BRUSH(RelativePath, ...)	FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define IMAGE_BRUSH_SVG( RelativePath, ... ) FSlateVectorImageBrush(Style->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...)	FSlateBoxBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...)		FSlateFontInfo(Style->RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...)		FSlateFontInfo(Style->RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon16x24(16.0f, 24.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon25x25(25.0f, 25.0f);
const FVector2D Icon22x28(22.0f, 28.0f);
const FVector2D Icon30x30(30.0f, 30.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon40x25(40.0f, 25.0f);
const FVector2D Icon50x50(50.0f, 50.0f);
const FVector2D Icon60x60(60.0f, 60.0f);
const FVector2D Icon120x120(120.0f, 120.0f);
const FVector2D Icon128x128(120.0f, 120.0f);

TSharedRef<FSlateStyleSet> FPaperZDEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	FString ContentDir = IPluginManager::Get().FindPlugin("PaperZD")->GetContentDir();
	Style->SetContentRoot(ContentDir);

	//Util
	Style->Set("PaperZDEditor.WhiteBrush", new FSlateColorBrush(FLinearColor(1.0f, 1.0f, 1.0f)));

	//AnimDataSourceGrid colors
	{
// 		Style->Set("PaperZDEditor.AnimDataSourceGrid.Regular", MakeShareable(new FLinearColor(0.72f, 0.72f, 0.72f, 1.f)));
// 		Style->Set("PaperZDEditor.AnimDataSourceGrid.Highlight", MakeShareable(new FLinearColor(0.701f, 0.225f, 0.003f)));
// 		Style->Set("PaperZDEditor.AnimDataSourceGrid.Pressed", MakeShareable(new FLinearColor(0.701f, 0.225f, 0.003f)));
// 		Style->Set("PaperZDEditor.AnimDataSourceGrid.Preview", MakeShareable(new FLinearColor(FColor(0xff33dd33))));
	}

	//Switcher
	Style->Set("PaperZDEditor.ModeSwitcher.Blueprint", new IMAGE_BRUSH_SVG("Icons/Switcher/BlueprintMode", Icon20x20));
	Style->Set("PaperZDEditor.ModeSwitcher.Animation", new IMAGE_BRUSH_SVG("Icons/Switcher/AnimationMode", Icon20x20));
	Style->Set("PaperZDEditor.ModeSwitcher.Separator", new IMAGE_BRUSH("Icons/Switcher/PipelineSeparator", Icon16x24));

	//Sequences
	Style->Set("PaperZDEditor.Sequences.Avatar", new IMAGE_BRUSH("Icons/Graph/Sequence", Icon40x40));

	//Asset Icons
	Style->Set("ClassThumbnail.PaperZDAnimBP", new IMAGE_BRUSH("Icons/AnimBP_Thumbnail", Icon120x120));
	Style->Set("ClassThumbnail.PaperZDAnimInstance", new IMAGE_BRUSH("Icons/AnimBP_Thumbnail", Icon120x120));
	Style->Set("ClassThumbnail.PaperZDAnimationSource", new IMAGE_BRUSH("Icons/AnimSource_Thumbnail", Icon120x120));
	Style->Set("ClassThumbnail.PaperZDAnimNotify", new IMAGE_BRUSH("Icons/AnimNotify_Thumbnail", Icon120x120));
	Style->Set("ClassThumbnail.PaperZDAnimNotifyState", new IMAGE_BRUSH("Icons/AnimNotifyState_Thumbnail", Icon120x120));
	Style->Set("ClassThumbnail.PaperZDAnimSequence", new IMAGE_BRUSH("Icons/AnimSequence_Thumbnail", Icon120x120));

	//Graph Related
	Style->Set("PaperZDEditor.Graph.TransitionNode.Icon", new IMAGE_BRUSH("Icons/Graph/Trans_Arrow", Icon25x25));
	Style->Set("PaperZDEditor.Graph.TransitionNode.Body", new IMAGE_BRUSH("Icons/Graph/Trans_Body", Icon30x30));
	Style->Set("PaperZDEditor.Graph.TransitionNode.Body_Seq", new IMAGE_BRUSH("Icons/Graph/Trans_Seq", Icon40x25));

	Style->Set("PaperZDEditor.Graph.TransitionNode.Icon", new IMAGE_BRUSH("Icons/Graph/Trans_Arrow", Icon25x25));
	Style->Set("PaperZDEditor.Graph.TransitionNode.Body", new IMAGE_BRUSH("Icons/Graph/Trans_Body", Icon30x30));
	Style->Set("PaperZDEditor.Graph.TransitionNode.Body_Seq", new IMAGE_BRUSH("Icons/Graph/Trans_Seq", Icon40x25));

	Style->Set("PaperZDEditor.Graph.SinkNode.ResultImage", new IMAGE_BRUSH("Icons/Graph/AnimationNode_Result_128x", Icon128x128));

	Style->Set("PaperZDEditor.Graph.AnimationPin.Connected", new IMAGE_BRUSH("Icons/Graph/AnimPin_Connected_22x28", Icon22x28));
	Style->Set("PaperZDEditor.Graph.AnimationPin.Disconnected", new IMAGE_BRUSH("Icons/Graph/AnimPin_Disconnected_22x28", Icon22x28));
	Style->Set("PaperZDEditor.Graph.AnimationPin.ConnectedHovered", new IMAGE_BRUSH("Icons/Graph/AnimPin_Connected_22x28", Icon22x28, FLinearColor(0.8f, 0.8f, 0.8f)));
	Style->Set("PaperZDEditor.Graph.AnimationPin.DisconnectedHovered", new IMAGE_BRUSH("Icons/Graph/AnimPin_Disconnected_22x28", Icon22x28, FLinearColor(0.8f, 0.8f, 0.8f)));

	return Style;
}

#undef IMAGE_BRUSH
#undef IMAGE_BRUSH_SVG
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

#undef PARENT_DIR
